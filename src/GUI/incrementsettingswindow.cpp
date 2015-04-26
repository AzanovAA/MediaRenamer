#include "incrementsettingswindow.h"

#include <QtWidgets>

IncrementSettingsWindow::IncrementSettingsWindow()
{
    previewLabel = new QLabel(this);

    initInterface();
    updatePreview();

    connect(this, &QDialog::finished, this, [=](){emit settingsChanged(settings);});
}

void IncrementSettingsWindow::initInterface()
{
    QLabel *logoLabel = new QLabel(this);
    QLabel *titleLabel = new QLabel("Increment Settings");
    QPushButton *okButton = new QPushButton("OK", this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *titleLayout = new QHBoxLayout();

    logoLabel->setPixmap(QPixmap(":/resources/settings_logo.png"));

    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel {"
                              "font: bold 16px \"Arial\";"
                              "border: none;"
                              "background: transparent;"
                              "}");

    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setStyleSheet("QLabel {"
                                "font: 12px \"Arial\";"
                                "border: none;"
                                "background: transparent;"
                                "}");

    okButton->setFixedSize(150, 30);
    okButton->setStyleSheet("QPushButton {"
                            "font: bold 13px \"Arial\";"
                            "color: rgb(58, 58, 58);"
                            "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(245, 245, 245), stop:1 rgb(214, 214, 214));"
                            "border: 1px solid rgb(171, 171, 171);"
                            "border-radius: 5px"
                            "}"
                            "QPushButton:pressed {"
                            "background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 rgb(214, 214, 214), stop:1 rgb(230, 230, 230));"
                            "}");

    setStyleSheet("QDialog {"
                  "background-color: rgb(245, 245, 245);"
                  "}");

    titleLayout->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(logoLabel);
    titleLayout->addWidget(titleLabel);

    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(titleLayout);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(initSettingsFrame());
    mainLayout->addWidget(previewLabel);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(okButton, 0, Qt::AlignCenter);

    connect(okButton, SIGNAL(clicked()), SLOT(close()));
}

QFrame* IncrementSettingsWindow::initSettingsFrame()
{
    QFrame *frame = new QFrame(this);

    QLabel *beginNumberLabel = new QLabel("Begin Number At:", frame);
    QLabel *digitsCountLabel = new QLabel("Number of Digits:", frame);
    QLabel *incrementPlaceLabel = new QLabel("Place Increments:", frame);
    QLabel *separatorLabel = new QLabel("Separated By:", frame);

    QLineEdit *beginNumberEdit = new QLineEdit(frame);
    QLineEdit *digitsCountEdit = new QLineEdit(frame);

    QComboBox *incrementPlaceBox = new QComboBox(frame);
    QComboBox *separatorBox = new QComboBox(frame);

    beginNumberLabel->setAlignment(Qt::AlignRight);
    digitsCountLabel->setAlignment(Qt::AlignRight);
    incrementPlaceLabel->setAlignment(Qt::AlignRight);
    separatorLabel->setAlignment(Qt::AlignRight);

    QGridLayout *layout = new QGridLayout(frame);

    frame->setStyleSheet("QFrame {"
                         "background-color: rgb(237, 237, 237);"
                         "border-top: 1px solid rgb(210, 210, 210);"
                         "border-left: 1px solid rgb(224, 224, 224);"
                         "border-right: 1px solid rgb(224, 224, 224);"
                         "border-bottom: 1px solid rgb(233, 233, 233);"
                         "border-radius: 10px;"
                         "}"
                         "QLabel {"
                         "font: bold 13px \"Arial\";"
                         "border: none;"
                         "background: transparent;"
                         "}");

    layout->setContentsMargins(40, 15, 40, 15);
    layout->addWidget(beginNumberLabel, 0, 0);
    layout->addWidget(beginNumberEdit, 0, 1);
    layout->addWidget(digitsCountLabel, 1, 0);
    layout->addWidget(digitsCountEdit, 1, 1);
    layout->addWidget(incrementPlaceLabel, 2, 0);
    layout->addWidget(incrementPlaceBox, 2, 1);
    layout->addWidget(separatorLabel, 3, 0);
    layout->addWidget(separatorBox, 3, 1);

    beginNumberEdit->setValidator(new QRegExpValidator(QRegExp("\\d*"), this));
    digitsCountEdit->setValidator(new QIntValidator(1, 25, this));

    incrementPlaceBox->addItems(QStringList() << "After Name" << "Before Name");
    separatorBox->addItems(QStringList() << "Space" << "Dash" << "Period" << "Underscore" << "Nothing");

    beginNumberEdit->setText(QString::number(settings.beginNumber));
    digitsCountEdit->setText(QString::number(settings.digitsCount));

    switch (settings.place)
    {
    case Settings::IncrementSettings::After:
        incrementPlaceBox->setCurrentIndex(0);
        break;

    case Settings::IncrementSettings::Befor:
        incrementPlaceBox->setCurrentIndex(1);
        break;

    default:
        break;
    }

    if (settings.separator == " ")
        separatorBox->setCurrentIndex(0);
    else if (settings.separator == "-")
        separatorBox->setCurrentIndex(1);
    else if (settings.separator == ".")
        separatorBox->setCurrentIndex(2);
    else if (settings.separator == "_")
        separatorBox->setCurrentIndex(3);
    else if (settings.separator.isEmpty())
        separatorBox->setCurrentIndex(4);

    connect(beginNumberEdit, SIGNAL(textChanged(QString)), SLOT(beginNumberChanged(QString)));
    connect(digitsCountEdit, SIGNAL(textChanged(QString)), SLOT(digitsCountChanged(QString)));
    connect(incrementPlaceBox, SIGNAL(currentIndexChanged(QString)), SLOT(incrementPlaceChanged(QString)));
    connect(separatorBox, SIGNAL(currentIndexChanged(QString)), SLOT(separatorChanged(QString)));

    return frame;
}

void IncrementSettingsWindow::beginNumberChanged(const QString &number)
{
    settings.beginNumber = number.toInt();

    updatePreview();
}

void IncrementSettingsWindow::digitsCountChanged(const QString &count)
{
    settings.digitsCount = count.toInt();

    updatePreview();
}

void IncrementSettingsWindow::incrementPlaceChanged(const QString &place)
{
    if (place == "After Name")
        settings.place = Settings::IncrementSettings::After;
    else if (place == "Before Name")
        settings.place = Settings::IncrementSettings::Befor;

    updatePreview();
}

void IncrementSettingsWindow::separatorChanged(const QString &separator)
{
    if (separator == "Space")
        settings.separator = " ";
    else if (separator == "Dash")
        settings.separator = "-";
    else if (separator == "Period")
        settings.separator = ".";
    else if (separator == "Underscore")
        settings.separator = "_";
    else if (separator == "Nothing")
        settings.separator.clear();

    updatePreview();
}

void IncrementSettingsWindow::updatePreview()
{
    if (settings.digitsCount > 0)
    {
        QString preview;
        preview.sprintf(("%0" + QString::number(settings.digitsCount) + "d" ).toLatin1().data(), settings.beginNumber);

        switch (settings.place)
        {
        case Settings::IncrementSettings::After:
            preview = "Item Name" + settings.separator + preview + ".xxx";
            break;

        case Settings::IncrementSettings::Befor:
            preview = preview + settings.separator + "Item Name.xxx";
            break;

        default:
            preview = "Item Name" + settings.separator + preview + ".xxx";
            break;
        }

        previewLabel->setText("Preview:     " + preview);
    }
    else
        previewLabel->setText("Preview:     Item Name.xxx");
}
