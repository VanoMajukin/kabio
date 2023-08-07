/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#include "slider.h"
#include "clickablelabel.h"
#include <QtWidgets>

Slider::Slider(int min, int max, int def, int ticks, QString name, std::function<QString(int)> measure)
{
    QGridLayout *grid = new QGridLayout();
    setLayout(grid);
    grid->setRowStretch(1, 1);
    QLabel *minLabel = new QLabel(QString::number(min));
    QLabel *maxLabel = new QLabel(QString::number(max));
    nameLabel = new QLabel(name);
    measureLabel = new ClickableLabel(measure(def));
    
    nameLabel->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum));
    nameLabel->setWordWrap(true);
    minLabel->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignBottom);
    maxLabel->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);
    nameLabel->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);
    measureLabel->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignBottom);
   
    slider = new QSlider();
    slider->setOrientation(Qt::Orientation::Vertical);
    slider->setRange(min, max);
    slider->setTickInterval((max - min) / ticks);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setValue(def);
    grid->addWidget(measureLabel, 0, 0, 1, 2);
    grid->addWidget(slider, 1, 0, 2, 1);
    grid->addWidget(maxLabel, 1, 1);
    grid->addWidget(minLabel, 2, 1);
    grid->addWidget(nameLabel, 3, 0, 1, 2);
    
    connect(measureLabel, &ClickableLabel::doubleClicked, slider, [=]() {
        slider->setValue(def);
    });
    connect(slider, &QAbstractSlider::valueChanged, measureLabel, [=](int value) {
        measureLabel->setText(measure(value));
        emit valueChanged(value);
    });
}
