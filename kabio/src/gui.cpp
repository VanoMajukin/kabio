/*
Copyright 2022 George Bartolomey

Этот файл — часть Kabio.

Kabio — свободная программа: вы можете перераспространять ее и/или изменять ее на условиях Стандартной общественной лицензии GNU в том виде, в каком она была опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.

Kabio распространяется в надежде, что она будет полезной, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.

Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этой программой. Если это не так, см. <https://www.gnu.org/licenses/>.
*/

#include "gui.hpp"
#include "jack.hpp"
#include "notedetection.hpp"
#include "midi.hpp"
#include "widgets/slider.h"
#include <QDial>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QtConcurrent/QtConcurrent>
#include <chrono>
#include <QMessageBox>

namespace kabio
{
    GUI::GUI()
    {
        buffer_size = 1024;
        try {
            io = new Jack((char*)"kabio", buffer_size, [=](float *input, unsigned int nsamples, MIDI *out) {
                audio_process(input, nsamples, out);
            });
        }
        catch (QString err) {
            QMessageBox::critical(this, tr("kabio"),
                               err,
                               QMessageBox::Abort);
            exit(1);
        }
        
        QVBoxLayout *vbox = new QVBoxLayout();
        setLayout(vbox);
        QHBoxLayout *controls = new QHBoxLayout();
        vbox->addLayout(controls);
        Slider *silence_threshold = new Slider(-100, 0, -80, 10, tr("Silence threshold"),
                                              [](int v) { return QString::number(v) + " " + tr("dB"); });
        Slider *median_ring_size = new Slider(1, 17, 5, 16, tr("Median ring buffer size"),
                                              [](int v) { return QString::number(v); });
        Slider *buffer_size_slider = new Slider(8, 15, log2(buffer_size), 15 - 8, tr("Samples buffer size"),
                                              [this](int v) { return buffer_size_slider_display(v); });
        controls->addWidget(silence_threshold);
        controls->addWidget(median_ring_size);
        controls->addWidget(buffer_size_slider);
        
        pitch_view = new QProgressBar();
        vbox->addWidget(pitch_view);
        pitch_view->setMinimum(-50);
        pitch_view->setMaximum(50);
        pitch_view->setValue(0);
        pitch_view->setFormat("");
        
        note_label = new QLabel("-");
        {
            QFont font = note_label->font();
            font.setPointSize(font.pointSize() * 2);
            note_label->setFont(font);
        }
        vbox->addWidget(note_label);
        
        QCheckBox *pitch_mode = new QCheckBox(tr("Pitch mode"));
        vbox->addWidget(pitch_mode);
        
        
        note_detection = new NoteDetection(buffer_size, 2, io->sample_rate(), 5);
        connect(silence_threshold, &Slider::valueChanged, this, [this](int v) { note_detection->set_silence_treshold(v); });
        connect(median_ring_size, &Slider::valueChanged, this, [this](int v) { note_detection->set_median_size(v); });
        connect(buffer_size_slider, &Slider::valueChanged, this, [this](int v) { set_buffer_size(1 << v); });
        qRegisterMetaType<NoteDetection::Result>();
        connect(this, &GUI::current_note_changed, this, &GUI::current_note_changed_slot, Qt::QueuedConnection);
        connect(pitch_mode, &QCheckBox::stateChanged, this, [this](int v) {
            note_detection->pitch_mode = v == Qt::CheckState::Checked;
        });
        
        io->start();
    }
    
    GUI::~GUI()
    {
        delete io;
        delete note_detection;
    }
    
    void GUI::current_note_changed_slot(NoteDetection::Result note)
    {
        static QString dies = "♯";
        static QString notes[] = {
            tr("C", "note"),
            tr("C", "note") + dies,
            tr("D", "note"),
            tr("D", "note") + dies,
            tr("E", "note"),
            tr("F", "note"),
            tr("F", "note") + dies,
            tr("G", "note"),
            tr("G", "note") + dies,
            tr("A", "note"),
            tr("A", "note") + dies,
            tr("B", "note")
        };
        pitch_view->setValue(note.pitch * 100);
        QString note_string;
        if (note.velocity > 0)
            note_string = QString("%1%2")
                .arg(notes[note.note % 12])
                .arg(note.note / 12 - 1);
        else
            note_string = "—";
        note_label->setText(note_string);
    }
    
    void GUI::audio_process(float *input, unsigned int ns, MIDI *out)
    {
        NoteDetection::Result note = note_detection->analyze(input, ns);
        out->send_note(note);
        emit current_note_changed(note);
    }
    
    void GUI::set_buffer_size(unsigned int size)
    {
        buffer_size = size;
        io->set_buffer_size(size);
        note_detection->set_buffer_size(size);
    }
    
    QString GUI::buffer_size_slider_display(int buf_size_power)
    {
        unsigned int buf_size = 1 << buf_size_power;
        return QString("%1 %2")
            .arg(round((float)buf_size / (float)io->sample_rate() * 1000))
            .arg(tr("ms", "milliseconds"));
    }

};
