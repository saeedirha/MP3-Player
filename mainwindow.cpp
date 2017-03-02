#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ShuffleFlag = false;
    this->RepeatStat= RepeatFlags::All;

    ui->setupUi(this);

    connect(ui->actionPlay_Selected, SIGNAL(triggered()), ui->btnPlay, SLOT(click()));
    mPlayer = new QMediaPlayer();
    connect(mPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(ElapsedTime(qint64)));
    connect(mPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(RemaningTime(qint64)));

    ///----------- Adding Context menus to playlist table
    ui->TablePlayList->addAction(ui->actionAdd_files);
    ui->TablePlayList->addAction(ui->actionPlay_Selected);
    ui->TablePlayList->addAction(ui->actionAdd_to_Queue);
    ui->TablePlayList->addAction(ui->menuFile->addSeparator());
    ui->TablePlayList->addAction(ui->actionRemove_File);
    ui->TablePlayList->addAction(ui->actionCopy_to_Clipboard);
    ui->TablePlayList->addAction(ui->actionClear_all);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mPlayer;
}

//////////////Context Menu Buttons//////////////////////
///

//adding file(s) to playlist table
void MainWindow::on_actionAdd_files_triggered()
{
    QStringList list  = QFileDialog::getOpenFileNames(this,
                                                     tr("Select Files"), "Z:\\Music",
                                                     tr("MP3 Files (*.mp3)"));
    if(list.isEmpty())
        return;

    int counter = ui->TablePlayList->rowCount();
    ui->TablePlayList->setRowCount(counter);
    foreach(QString x, list)
    {
        ui->TablePlayList->insertRow(counter);
        int xbegin = x.lastIndexOf("/")+1;
        int xend = x.lastIndexOf('.') - xbegin ;
        QString title = x.mid( xbegin, xend);

        QTableWidgetItem *item_title = new QTableWidgetItem(title);
        QTableWidgetItem *item_path = new QTableWidgetItem(x);

        ui->TablePlayList->setItem(counter, 0, item_title);
        ui->TablePlayList->setItem(counter, 1, item_path);
        counter++;
        //qDebug() << xend;
    }
    ui->TablePlayList->resizeColumnToContents(0);
}

//remove single file from the playlist table
void MainWindow::on_actionRemove_File_triggered()
{
    QItemSelectionModel *selected = ui->TablePlayList->selectionModel();

    if(selected->hasSelection())
    {
        int m_current_row = selected->currentIndex().row();
        ui->TablePlayList->removeRow(m_current_row);
        mPlayer->stop();
    }
}

//clearing the Playlist table
void MainWindow::on_actionClear_all_triggered()
{
    ui->TablePlayList->clearContents();
    ui->TablePlayList->setRowCount(0);
}

//Saving File path to clipboard if needed
void MainWindow::on_actionCopy_to_Clipboard_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    QItemSelectionModel *selected = ui->TablePlayList->selectionModel();

    if(selected->hasSelection())
    {
        int m_current_row = selected->currentIndex().row();
        clipboard->setText(ui->TablePlayList->item(m_current_row, 1)->text());
        qDebug() << "Clipboard content: " << clipboard->text();
    }

}

//// --------------------- NOT IMPLEMENTED YET --------------------------//
void MainWindow::on_actionAdd_to_Queue_triggered()
{

}
///--------------------------------------------------------------------//

//////////////End of Context Menu Buttons//////////////////////

//////////////// Buttons /////////////////////////////////
void MainWindow::on_btnRepeat_clicked()
{
    RepeatStat++;

    if(RepeatStat == RepeatFlags::One )
    {
        ui->btnRepeat->setChecked(true);
        ui->statusBar->showMessage("Repeat One", 5*1000);
    }else if(RepeatStat == RepeatFlags::All)
    {
        ui->btnRepeat->setChecked(true);
        ui->statusBar->showMessage("Repeat All", 5*1000);
    }else{
        ui->btnRepeat->setChecked(false);
        ui->statusBar->showMessage("Repeat Off", 5*1000);
        RepeatStat=0;
    }

}

void MainWindow::on_btnShuffel_clicked()
{
    if(ui->btnShuffel->isChecked())
    {
        ShuffleFlag = true;
        ui->statusBar->showMessage("Shuffle selection is On", 5*1000);
    }else{
        ShuffleFlag = false;
        ui->statusBar->showMessage("Shuffle selection is Off", 5*1000);
    }
}

/////// Play Button

void MainWindow::on_btnPlay_clicked()
{
    QItemSelectionModel *selected = ui->TablePlayList->selectionModel();
    if(ui->TablePlayList->item(0, 0) ==0)
        return;

    int m_current_row=0;

    if(ShuffleFlag)
    {
        int randomValue = qrand() % ui->TablePlayList->rowCount();
        qDebug() << "Random song number:" << randomValue;
        ui->TablePlayList->selectRow(randomValue);
    }

    if(selected->hasSelection())
    {
        m_current_row = selected->currentIndex().row();

    }else{
        ui->TablePlayList->selectRow(m_current_row);
    }

    ui->statusBar->showMessage("[+]Now Playing: " + ui->TablePlayList->item(m_current_row, 0)->text(), 20*1000);
    QString filename  = ui->TablePlayList->item(m_current_row, 1)->text();
    mPlayer->setMedia(QUrl::fromLocalFile(filename));
    mPlayer->play();

}

/////// Pause Button

void MainWindow::on_btnPause_clicked()
{
    if(ui->btnPause->isChecked())
    {
        mPlayer->pause();
    }else{
        mPlayer->play();
    }
}


/////// Stop Button
void MainWindow::on_btnStop_clicked()
{
    mPlayer->stop();
}


/////// Mute Button
void MainWindow::on_btnMute_clicked()
{
    if(ui->btnMute->isChecked())
    {
        mPlayer->setMuted(true);
    }else{
        mPlayer->setMuted(false);
    }
}

/////// Next Button
void MainWindow::on_btnNext_clicked()
{
    QItemSelectionModel *selected = ui->TablePlayList->selectionModel();
    int m_current_row = selected->currentIndex().row();

    if(ui->TablePlayList->item(0, 0) ==0  &&  m_current_row == 0)
        return;

    if(RepeatStat == RepeatFlags::One)
    {
        ui->TablePlayList->selectRow(m_current_row);

    }else if( (RepeatStat == RepeatFlags::All) && (m_current_row == ui->TablePlayList->rowCount()-1) )
    {
        ui->TablePlayList->selectRow(0);
    }else{
        ui->TablePlayList->selectRow(m_current_row+1);
    }
    on_btnPlay_clicked();
}

/////// Previous Button
void MainWindow::on_btnPrev_clicked()
{
    QItemSelectionModel *selected = ui->TablePlayList->selectionModel();
    int m_current_row = selected->currentIndex().row();

    if(ui->TablePlayList->item(0, 0) ==0  &&  m_current_row == 0)
        return;

    if(RepeatStat == RepeatFlags::One)
    {
        ui->TablePlayList->selectRow(m_current_row);

    }else if( (RepeatStat == RepeatFlags::All) && (m_current_row == 0) )
    {
        ui->TablePlayList->selectRow(ui->TablePlayList->rowCount()-1);
    }else{
        ui->TablePlayList->selectRow(m_current_row-1);
    }
    on_btnPlay_clicked();

}

//////////////// End of Buttons /////////////////////////////////

////////////////////////////////////////////


//Seek slider
void MainWindow::on_SeekSlider_sliderMoved(int position)
{
    mPlayer->setPosition(position);
}

//Volume Slider
void MainWindow::on_VolumeSlider_sliderMoved(int position)
{
    mPlayer->setVolume(position);
}


//Defined slots

void MainWindow::ElapsedTime(qint64 x)
{

    QTime t = QTime(0,0,0);
    t =  t.addMSecs(x);
    ui->lblTimeElsp->setText(t.toString("mm:ss"));
    ui->SeekSlider->setValue(x);
    //qDebug() << "Duration" << mPlayer->duration();

    if(x!=0 && x==mPlayer->duration() )
    {
        if(RepeatStat == RepeatFlags::None)
        {
            mPlayer->stop();
            return;
        }

        on_btnNext_clicked();
    }
    //qDebug() << RepeatStat;

}
/// Slot to update interface
void MainWindow::RemaningTime(qint64 x)
{
    QTime t = QTime(0,0,0);
    t =  t.addMSecs(x);
    ui->lblTimeRemaining->setText(t.toString("mm:ss"));
    //qDebug() << t;
    ui->SeekSlider->setRange(0, x);
}

