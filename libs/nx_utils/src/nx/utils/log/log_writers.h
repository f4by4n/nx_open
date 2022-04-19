// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <fstream>
#include <future>

#include <nx/utils/log/log_level.h>
#include <nx/utils/thread/mutex.h>

#include <QtCore/QDateTime>
#include <QtCore/QFileInfo>
#include <QtCore/QLockFile>

namespace nx {
namespace utils {
namespace log {

static constexpr int kMaxLogRotation = 999;
static constexpr qint64 kDefaultMaxLogVolumeSizeB = 500 * 1024 * 1024;
static constexpr qint64 kDefaultMaxLogFileSizeB = 10 * 1024 * 1024;
static constexpr std::chrono::seconds kDefaultMaxLogFileTimePeriodS = std::chrono::seconds::zero();

class NX_UTILS_API AbstractWriter
{
public:
    virtual ~AbstractWriter() = default;
    virtual void write(Level level, const QString& message) = 0;
};

/**
 * Writes to std out.
 */
class NX_UTILS_API StdOut: public AbstractWriter
{
public:
    virtual void write(Level level, const QString& message) override;

private:
    static void writeImpl(Level level, const QString& message);
};

/**
 * File writer with backup rotation.
 */
class NX_UTILS_API File: public AbstractWriter
{
public:
    static constexpr char kExtensionWithSeparator[] = ".log";
    static constexpr char kTmpExtensionWithSeparator[] = ".log.tmp";
    static constexpr char kRotateExtensionWithSeparator[] = ".log.zip";
    static constexpr char kRotateTmpExtensionWithSeparator[] = ".log.zip.tmp";

    static QString makeFileName(QString fileName, size_t backupNumber);
    static QString makeBaseFileName(QString path);

    struct Settings
    {
        QString name; /**< Base file name with path. */
        qint64 maxVolumeSizeB = kDefaultMaxLogVolumeSizeB; /**< Maximum volume size. */
        qint64 maxFileSizeB = kDefaultMaxLogFileSizeB; /**< Maximum file size. */
        std::chrono::seconds maxFileTimePeriodS = kDefaultMaxLogFileTimePeriodS; /**< Maximum file duration in time. */
    };

    File(Settings settings);
    virtual void write(Level level, const QString& message) override;
    QString getFileName(size_t backupNumber = 0) const;

    void setSettings(const Settings& settings);

private:
    bool openFile();
    void rotateIfNeeded(nx::Locker<nx::Mutex>* lock);
    void archiveLeftOvers(nx::Locker<nx::Mutex>* lock);
    bool queueToArchive(nx::Locker<nx::Mutex>* lock);
    bool isCurrentLimitReached(nx::Locker<nx::Mutex>* lock);
    void archive(QString fileName, QString archiveName);
    qint64 totalVolumeSize();

private:
    Settings m_settings;
    nx::Mutex m_mutex;
    std::fstream m_file;
    QFileInfo m_fileInfo;
    QDateTime m_fileOpenTime;
    QLockFile m_volumeLock;
    std::future<void> m_archive;
    int m_archiveQueue = 0;
};

/**
 * Writes messages to internal buffer.
 */
class NX_UTILS_API Buffer: public AbstractWriter
{
public:
    virtual void write(Level level, const QString& message) override;

    std::vector<QString> takeMessages();
    void clear();

private:
    nx::Mutex m_mutex;
    std::vector<QString> m_messages;
};

/**
 * Does nothing, all messages are lost.
 */
class NX_UTILS_API NullDevice: public AbstractWriter
{
public:
    virtual void write(Level level, const QString& message) override;
};

} // namespace log
} // namespace utils
} // namespace nx
