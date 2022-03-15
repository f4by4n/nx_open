// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#include "log_settings.h"

#include <QtCore/QSettings>

#include <nx/utils/deprecated_settings.h>
#include <nx/utils/string.h>
#include <nx/utils/std/filesystem.h>
#include <nx/utils/time.h>

#include "format.h"
#include "assert.h"

namespace nx {
namespace utils {
namespace log {

bool LoggerSettings::parse(const QString& str)
{
    bool parseSucceeded = true;

    std::multimap<std::string, std::string> params;
    splitNameValuePairs(
        str.toStdString(), ',', '=',
        std::inserter(params, params.end()),
        GroupToken::doubleQuotes | GroupToken::squareBrackets);
    for (const auto& param: params)
    {
        if (param.first == "file")
        {
            const auto filePath = nx::utils::filesystem::path(param.second);

            logBaseName = QString::fromStdString(filePath.filename().string());
            if (filePath.has_parent_path())
                directory = QString::fromStdString(filePath.parent_path().string());
        }
        else if (param.first == "level")
        {
            parseSucceeded = parseSucceeded && level.parse(param.second.c_str());
        }
        else if (param.first == "dir")
        {
            directory = param.second.c_str();
        }
        else if (param.first == kMaxLogVolumeSizeSymbolicName)
        {
            bool ok = false;
            maxVolumeSizeB = stringToBytes(param.second, &ok);
            parseSucceeded = parseSucceeded && ok;
        }
        else if (param.first == kMaxLogFileSizeSymbolicName)
        {
            bool ok = false;
            maxFileSizeB = stringToBytes(param.second, &ok);
            parseSucceeded = parseSucceeded && ok;
        }
        else if (param.first == kMaxLogFileTimePeriodSymbolicName)
        {
            const auto duration = nx::utils::parseDuration(param.second);
            if (duration)
                maxFileTimePeriodS = std::chrono::duration_cast<std::chrono::seconds>(duration.value());
            parseSucceeded = parseSucceeded && duration;
        }
    }

    if (!NX_ASSERT(maxVolumeSizeB >= maxFileSizeB))
        maxVolumeSizeB = maxFileSizeB;

    return parseSucceeded;
}

void LoggerSettings::updateDirectoryIfEmpty(const QString& logDirectory)
{
    if (directory.isEmpty())
        directory = logDirectory;
}

bool LoggerSettings::operator==(const LoggerSettings& right) const
{
    return level == right.level
        && directory == right.directory
        && maxFileSizeB == right.maxFileSizeB
        && maxVolumeSizeB == right.maxVolumeSizeB
        && maxFileTimePeriodS == right.maxFileTimePeriodS
        && logBaseName == right.logBaseName;
}

//-------------------------------------------------------------------------------------------------

Settings::Settings(QSettings* settings)
{
    NX_ASSERT(settings);
    if (!settings)
        return;

    auto maxLogVolumeSizeB = settings->value(kMaxLogVolumeSizeSymbolicName, kDefaultMaxLogVolumeSizeB).toLongLong();
    const auto maxLogFileSizeB = settings->value(kMaxLogFileSizeSymbolicName, kDefaultMaxLogFileSizeB).toLongLong();
    const auto maxLogFileTimePeriodS = std::chrono::seconds(
        settings->value(kMaxLogFileTimePeriodSymbolicName, (uint)kDefaultMaxLogFileTimePeriodS.count()).toUInt());

    if (!NX_ASSERT(maxLogVolumeSizeB >= maxLogFileSizeB))
        maxLogVolumeSizeB = maxLogFileSizeB;

    for (const auto& group: settings->childGroups())
    {
        LoggerSettings logger;
        logger.logBaseName = group;
        logger.maxVolumeSizeB = maxLogVolumeSizeB;
        logger.maxFileSizeB = maxLogFileSizeB;
        logger.maxFileTimePeriodS = maxLogFileTimePeriodS;
        logger.level.primary = Level::none;

        settings->beginGroup(group);
        for (const QString& levelKey: settings->childKeys())
        {
            const Level level = levelFromString(levelKey);
            const QVariant& value = settings->value(levelKey);

            switch (value.type())
            {
                case QVariant::String:
                {
                    const QString& valueString = value.toString().trimmed();
                    if (valueString == "*")
                        logger.level.primary = level;
                    else
                        logger.level.filters[Filter(valueString)] = level;
                    break;
                }
                case QVariant::StringList:
                {
                    for (const QString& valueString: value.toStringList())
                        logger.level.filters[Filter(valueString.trimmed())] = level;
                    break;
                }
                default:
                    break;
            }
        }
        settings->endGroup();

        loggers.push_back(std::move(logger));
    }
}

void Settings::load(const QnSettings& settings, const QString& prefix)
{
    int logSettingCount = 0;

    // Parsing every prefix/logger argument.
    const auto allArgs = settings.allArgs();
    for (const auto& arg: allArgs)
    {
        if (!arg.first.startsWith(prefix))
            continue;
        ++logSettingCount;

        if (arg.first.startsWith(nx::format("%1/logger").args(prefix).toQString()))
        {
            LoggerSettings loggerSettings;
            loggerSettings.parse(arg.second);
            loggers.push_back(std::move(loggerSettings));
        }
    }

    // If there are more prefix/* arguments, adding one more logger.
    if ((int) loggers.size() < logSettingCount)
        loadCompatibilityLogger(settings, prefix);
}

void Settings::updateDirectoryIfEmpty(const QString& logDirectory)
{
    for (auto& logger: loggers)
        logger.updateDirectoryIfEmpty(logDirectory);
}

void Settings::loadCompatibilityLogger(
    const QnSettings& settings,
    const QString& prefix)
{
    LoggerSettings loggerSettings;

    const auto makeKey =
        [&prefix](const char* key) { return QString(nx::format("%1/%2").arg(prefix).arg(key)); };

    QString logLevelStr = settings.value(makeKey("logLevel")).toString();
    if (logLevelStr.isEmpty())
        logLevelStr = settings.value("log-level").toString();
    if (logLevelStr.isEmpty())
        logLevelStr = settings.value("ll").toString();
    loggerSettings.level.parse(logLevelStr);

    loggerSettings.directory = settings.value(makeKey("logDir")).toString();
    loggerSettings.maxVolumeSizeB = nx::utils::stringToBytes(
        settings.value(makeKey(kMaxLogVolumeSizeSymbolicName)).toString().toStdString(),
        loggerSettings.maxVolumeSizeB);
    loggerSettings.maxFileSizeB = nx::utils::stringToBytes(
        settings.value(makeKey(kMaxLogFileSizeSymbolicName)).toString().toStdString(),
        loggerSettings.maxFileSizeB);

    if (!NX_ASSERT(loggerSettings.maxVolumeSizeB >= loggerSettings.maxFileSizeB))
        loggerSettings.maxVolumeSizeB = loggerSettings.maxFileSizeB;

    const auto duration = nx::utils::parseDuration(
        settings.value(makeKey(kMaxLogFileTimePeriodSymbolicName)).toString().toStdString());
    if (duration)
        loggerSettings.maxFileTimePeriodS = std::chrono::duration_cast<std::chrono::seconds>(duration.value());

    loggerSettings.logBaseName = settings.value(makeKey("baseName")).toString();
    if (loggerSettings.logBaseName.isEmpty())
        loggerSettings.logBaseName = settings.value("log-file").toString();
    if (loggerSettings.logBaseName.isEmpty())
        loggerSettings.logBaseName = settings.value("lf").toString();

    loggers.push_back(std::move(loggerSettings));
}

} // namespace log
} // namespace utils
} // namespace nx
