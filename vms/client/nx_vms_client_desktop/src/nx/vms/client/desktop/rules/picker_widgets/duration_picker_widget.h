// Copyright 2018-present Network Optix, Inc. Licensed under MPL 2.0: www.mozilla.org/MPL/2.0/

#pragma once

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>

#include <nx/vms/rules/action_builder_fields/optional_time_field.h>
#include <nx/vms/rules/action_builder_fields/time_field.h>
#include <nx/vms/rules/actions/bookmark_action.h>
#include <nx/vms/rules/utils/field.h>
#include <nx/vms/rules/utils/type.h>
#include <ui/common/read_only.h>
#include <ui/widgets/business/time_duration_widget.h>

#include "picker_widget.h"
#include "picker_widget_strings.h"
#include "picker_widget_utils.h"

namespace nx::vms::client::desktop::rules {

/** Used for types that could be represented as a time period. */
template<typename F>
class DurationPicker: public FieldPickerWidget<F>
{
public:
    DurationPicker(QnWorkbenchContext* context, CommonParamsWidget* parent):
        FieldPickerWidget<F>(context, parent)
    {
        auto contentLayout = new QHBoxLayout;

        m_timeDurationWidget = new TimeDurationWidget;
        m_timeDurationWidget->setSizePolicy(
            QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred));

        contentLayout->addWidget(m_timeDurationWidget);

        m_contentWidget->setLayout(contentLayout);

        connect(
            m_timeDurationWidget,
            &TimeDurationWidget::valueChanged,
            this,
            &DurationPicker::onValueChanged);
    }

protected:
    PICKER_WIDGET_COMMON_USINGS
    TimeDurationWidget* m_timeDurationWidget{nullptr};

    virtual void onDescriptorSet() override
    {
        FieldPickerWidget<F>::onDescriptorSet();

        auto maxIt = m_fieldDescriptor->properties.constFind("max");
        if (maxIt != m_fieldDescriptor->properties.constEnd())
            m_timeDurationWidget->setMaximum(maxIt->template value<std::chrono::seconds>().count());

        auto minIt = m_fieldDescriptor->properties.constFind("min");
        m_timeDurationWidget->setMinimum(minIt == m_fieldDescriptor->properties.constEnd()
            ? 0
            : minIt->template value<std::chrono::seconds>().count());
    }

    virtual void updateUi()
    {
        auto field = theField();

        if (m_fieldDescriptor->linkedFields.contains(vms::rules::utils::kDurationFieldName))
        {
            const auto durationField =
                FieldPickerWidget<F>::template getActionField<vms::rules::OptionalTimeField>(
                    vms::rules::utils::kDurationFieldName);
            if (!NX_ASSERT(durationField))
                return;

            const auto hasDuration =
                durationField->value() != vms::rules::OptionalTimeField::value_type::zero();
            if (hasDuration && field->value() != F::value_type::zero())
            {
                field->setValue(F::value_type::zero());
                return;
            }

            this->setVisible(!hasDuration);
        }

        QSignalBlocker blocker{m_timeDurationWidget};
        m_timeDurationWidget->setValue(
            std::chrono::duration_cast<std::chrono::seconds>(field->value()).count());
    }

private:
    void onValueChanged()
    {
        theField()->setValue(std::chrono::seconds{m_timeDurationWidget->value()});
    }
};

template<typename F>
class OptionalDurationPicker: public DurationPicker<F>
{
public:
    OptionalDurationPicker(QnWorkbenchContext* context, CommonParamsWidget* parent):
        DurationPicker<F>(context, parent)
    {
        m_label->setVisible(false);
        m_checkBox = new QCheckBox;
        static_cast<QHBoxLayout*>(m_contentWidget->layout())->insertWidget(0, m_checkBox);

        connect(
            m_checkBox,
            &QCheckBox::stateChanged,
            this,
            &OptionalDurationPicker<F>::onStateChanged);

        setMinimumHeight(m_timeDurationWidget->minimumHeight());
    }

private:
    PICKER_WIDGET_COMMON_USINGS
    using DurationPicker<F>::m_timeDurationWidget;
    using DurationPicker<F>::setMinimumHeight;

    QCheckBox* m_checkBox{nullptr};
    bool m_isDurationField{false};

    std::chrono::microseconds defaultValue() const
    {
        return m_fieldDescriptor->properties.value("default").template value<std::chrono::seconds>();
    }

    virtual void onDescriptorSet() override
    {
        DurationPicker<F>::onDescriptorSet();

        m_checkBox->setText(m_fieldDescriptor->displayName);

        m_timeDurationWidget->addDurationSuffix(QnTimeStrings::Suffix::Minutes);
        m_timeDurationWidget->addDurationSuffix(QnTimeStrings::Suffix::Hours);
        m_timeDurationWidget->addDurationSuffix(QnTimeStrings::Suffix::Days);
    }

    void updateUi() override
    {
        auto field = theField();

        if (m_fieldDescriptor->fieldName == vms::rules::utils::kDurationFieldName)
        {
            const bool isProlonged =
                parentParamsWidget()->eventDescriptor()->flags.testFlag(vms::rules::ItemFlag::prolonged);

            if (!isProlonged && field->value() == F::value_type::zero())
            {
                field->setValue(defaultValue());
                return;
            }

            setReadOnly(m_checkBox, !isProlonged);
        }

        DurationPicker<F>::updateUi();

        const bool hasInterval = field->value() != F::value_type::zero();

        if (m_fieldDescriptor->fieldName == vms::rules::utils::kIntervalFieldName
            || m_fieldDescriptor->fieldName == vms::rules::utils::kPlaybackTimeFieldName)
        {
            m_timeDurationWidget->setVisible(hasInterval);

            QString hint;
            if (m_fieldDescriptor->fieldName == vms::rules::utils::kIntervalFieldName)
            {
                hint = DurationPickerWidgetStrings::intervalOfActionHint(
                    /*isInstant*/ !hasInterval);
            }
            else
            {
                hint = DurationPickerWidgetStrings::playbackTimeHint(
                    /*isLive*/ !hasInterval);
            }

            m_checkBox->setText(QString{"%1: %2"}.arg(m_fieldDescriptor->displayName).arg(hint));
        }
        else
        {
            m_timeDurationWidget->setEnabled(hasInterval);
        }

        const QSignalBlocker blocker{m_checkBox};
        m_checkBox->setChecked(hasInterval);
    }

    void onStateChanged()
    {
        theField()->setValue(m_checkBox->isChecked() ? defaultValue() : F::value_type::zero());
    }
};

} // namespace nx::vms::client::desktop::rules
