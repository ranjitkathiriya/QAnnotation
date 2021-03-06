#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clickable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->label->setContentsMargins(0,0,0,0);
    ui->label->setAlignment (Qt::AlignCenter);
    ui->label->setMouseTracking(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Mouse_Current_Pos()
{
    QPixmap m_logo_pic_buff = m_logo_pic;
    QPainter painter(&m_logo_pic_buff);
    QPen Red((Qt::green),8);
    painter.setPen(Red);
    painter.setFont(QFont("times",40,QFont::ExtraBold));
    painter.drawRect(point1.x(), point1.y(), ui->label->rectWidth, ui->label->rectHeight);

    ui->label->setPixmap(m_logo_pic_buff);

}

void MainWindow::Mouse_Pressed()
{
    point1 = QPoint(ui->label->x, ui->label->y);
    drag = 1;
}

void MainWindow::Mouse_Release()
{
    QPainter painter(&m_logo_pic);
    QPen Red((Qt::green),8);
    painter.setPen(Red);
    painter.setFont(QFont("times",40,QFont::ExtraBold));
    painter.drawRect(point1.x(), point1.y(), ui->label->rectWidth, ui->label->rectHeight);

    ui->label->setPixmap(m_logo_pic);

    qDebug()<< point1.x() << point1.y() << ui->label->rectWidth << ui->label->rectHeight;
    qDebug() << ui->label->width() << ui->label->height();

    rect = QRect(point1.x(),point1.y(),ui->label->rectWidth,ui->label->rectHeight);

    ui->label->setCursor(Qt::ArrowCursor);



    float x_scale = (float)image_width / (float)ui->label->width();
    float y_scale = (float)image_height / (float)ui->label->height();


    int x = qRound(rect.x() * x_scale);
    int y = qRound(rect.y() * y_scale);
    int w = qRound(rect.width() * x_scale);
    int h = qRound(rect.height() * y_scale);

    qDebug() << x << y << x+w <<y+h;

    rect_scale = QRect(x,y,x+w,y+h);


    double dw = (double)1./(double)image_width;
    double dh = (double)1./(double)image_height;

    double x_sclae = (((double)x + (double)x+w)/(double)2.0) * dw;
    double y_sclae = (((double)y + (double)y+h)/(double)2.0) * dh;
    double x_w_sclae = (((double)x+w - (double)x)) * dw;
    double y_h_sclae = (((double)y+h - (double)y)) * dh;



    result += "0 "+QString::number(x_sclae, 'f', 6) +" " + QString::number(y_sclae, 'f', 6) +" "+ QString::number(x_w_sclae, 'f', 6)+" "+ QString::number(y_h_sclae, 'f', 6)+"\n";


    // saveTextfile code

    disconnect(ui->label, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_Current_Pos()));
    disconnect(ui->label, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));
    disconnect(ui->label, SIGNAL(Mouse_Release()), this, SLOT(Mouse_Release()));

}


void MainWindow::viewListImageview(QString imagepath)
{
    m_logo_pic.load(path+"/"+imagepath);

    image_width = m_logo_pic.width();
    image_height = m_logo_pic.height();

    qDebug()<< "ON LOAD::: "<< image_height << image_width;
    m_logo_pic = m_logo_pic.scaled(ui->label->width(), ui->label->height());
    ui->label->setPixmap(m_logo_pic);
    // ROS Service Here.

    QStringList query = imagepath.split(".");
    updatetxt(path+"/"+query[0]);
}

// List view Double click for opening image in lbl.
void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{

    viewListImageview(index.data().toString());
}

// Directory for image select
void MainWindow::on_actionSave_Label_Directory_triggered()
{
    path = QFileDialog::getExistingDirectory(this, tr("Choose catalog"), ".", QFileDialog::ReadOnly);
//    qDebug() << "Hello world!" << path;

    QStringListModel *model = new QStringListModel(this);
    QDir directory(path+"/");
    QStringList images = directory.entryList(QStringList() <<"*.png" << "*.jpg" << "*.JPG",QDir::Files);
    model->setStringList(images);
    ui->listView->setModel(model);
}

// Next Image
void MainWindow::on_actionNext_triggered()
{
    QModelIndex mi = ui->listView->currentIndex();

    if(mi.row() < ui->listView->model()->rowCount() - 1) {
        ui->listView->setCurrentIndex(mi.sibling(mi.row()+1,mi.column()));
        viewListImageview(ui->listView->currentIndex().data().toString());
    }
}

//Previous Image
void MainWindow::on_actionPrevious_triggered()
{
    QModelIndex mi = ui->listView->currentIndex();

    if(mi.row() > 0) {
        ui->listView->setCurrentIndex(mi.sibling(mi.row()-1,mi.column()));
        viewListImageview(ui->listView->currentIndex().data().toString());
    }
}

// Quit Application
void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

// Clear all content on screen trigger
void MainWindow::on_actionClose_triggered()
{
    qDebug() << "Closes";
    if(ui->listView->currentIndex().row() > 0) {
        ui->listView->model()->removeRows(0,ui->listView->model()->rowCount());
    //    ui->listView_2->model()->removeRows(0,ui->listView_2->model()->rowCount());
        ui->label->clear();
    }
}

// About Qt
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::aboutQt(this);
}

// About software
void MainWindow::on_actionAbout_2_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("This application is developed by DairyRobotic. \n\n Automatic annotation for cow teat.\n\n Author: Ranjit Kathiriya\n Email: ranjitkathiriya@gmail.com");
    msgBox.exec();
}

// Action tutorials
void MainWindow::on_actionTutorial_triggered()
{
    QPainter painter(&m_logo_pic);
    QPen Red((Qt::green),10);
    painter.setPen(Red);
    painter.drawLine(10,10,50,50);
    ui->label->setPixmap(m_logo_pic);

    // For fixing GUI image issue
    const int w = ui->label->width();
    const int h = ui->label->height();
    ui->label->setPixmap(m_logo_pic.scaled(w,h,Qt::KeepAspectRatio));
}

void MainWindow::updatetxt(QString fullpath){
    QFile file(fullpath+".txt");
    QString line;
    int line_count=0;
    QStringList list;
//    model1 = new QStringListModel(this);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            line_count++;
            QStringList list1 = line.split(' ');

            int x = qRound((list1[1].toDouble() - list1[3].toDouble() / 2) * m_logo_pic.width());
            int y = qRound((list1[2].toDouble() - list1[4].toDouble() / 2) * m_logo_pic.height());
            int w = qRound((list1[1].toDouble() + list1[3].toDouble() / 2) * m_logo_pic.width());
            int h = qRound((list1[2].toDouble() + list1[4].toDouble() / 2) * m_logo_pic.height());

            QPainter painter(&m_logo_pic);
            QPen Red((Qt::black),8);
            painter.setPen(Red);


            painter.setFont(QFont("times",40,QFont::ExtraBold));

            painter.drawRect(x,y,w-x,h-y);
            painter.drawText(x, y-10, "Label "+ QString::number(line_count));
            ui->label->setPixmap(m_logo_pic);

            list<< "Label_"+ QString::number(line_count);
        }
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
        file.close();
    }
    else{
        list<<"Null";
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
        file.close();
    }
}

void MainWindow::on_listView_2_doubleClicked(const QModelIndex &index)
{
    count_list2 = index.data().toString().split("_")[1].toInt();
    QStringList pathLast = ui->listView->currentIndex().data().toString().split(".");
    QFile file(path+"/"+pathLast[0]+".txt");
    QString line;
    int line_count=1;

    qDebug()<<"WIDTH:::: "<< m_logo_pic.width();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            if(index.data().toString().split("_")[1].toInt() == line_count){
                QStringList list1 = line.split(' ');
                int x = qRound((list1[1].toDouble() - list1[3].toDouble() / 2) * m_logo_pic.width());
                int y = qRound((list1[2].toDouble() - list1[4].toDouble() / 2) * m_logo_pic.height());
                int w = qRound((list1[1].toDouble() + list1[3].toDouble() / 2) * m_logo_pic.width());
                int h = qRound((list1[2].toDouble() + list1[4].toDouble() / 2) * m_logo_pic.height());
                QPainter painter(&m_logo_pic);
                QPen Red((Qt::green),8);
                painter.setPen(Red);
                painter.setFont(QFont("times",40,QFont::ExtraBold));
                painter.drawRect(x,y,w-x,h-y);
                painter.drawText(x, y-10, "Label "+ QString::number(line_count));
                ui->label->setPixmap(m_logo_pic);
            }else{
                QStringList list1 = line.split(' ');
                int x = qRound((list1[1].toDouble() - list1[3].toDouble() / 2) * m_logo_pic.width());
                int y = qRound((list1[2].toDouble() - list1[4].toDouble() / 2) * m_logo_pic.height());
                int w = qRound((list1[1].toDouble() + list1[3].toDouble() / 2) * m_logo_pic.width());
                int h = qRound((list1[2].toDouble() + list1[4].toDouble() / 2) * m_logo_pic.height());
                QPainter painter(&m_logo_pic);
                QPen Red((Qt::black),8);
                painter.setPen(Red);
                painter.setFont(QFont("times",40,QFont::ExtraBold));
                painter.drawRect(x,y,w-x,h-y);
                painter.drawText(x, y-10, "Label "+ QString::number(line_count));
                ui->label->setPixmap(m_logo_pic);
            }
            line_count++;
        }
    }
    file.close();
}

void MainWindow::on_actionOpen_triggered()
{
    QString selfilter = tr("JPEG (*.jpg *.jpeg)");
    QString fileName = QFileDialog::getOpenFileName(
            this,
            "Select File",
            "/home",
            tr("All files (*.*);;JPEG (*.jpg *.jpeg);;TIFF (*.tif)" ),
            &selfilter);

    QStringListModel *model = new QStringListModel(this);
    int pos = fileName.lastIndexOf(QChar('/'));
    qDebug() << fileName.split("/").takeLast();
    path = fileName.left(pos);
    QStringList list = model->stringList();
    list.append(fileName.split("/").takeLast());
    model->setStringList(list);
    ui->listView->setModel(model);
}

void MainWindow::on_actionReset_All_triggered()
{
    qDebug()<< ui->listView->currentIndex().data().toString().split(".")[0];

    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete labeling file?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
        QFile::remove(path+"/"+ui->listView->currentIndex().data().toString().split(".")[0]+".txt");

        QStringList list;
        list<<"Null";
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
      }
      // Call images label rect function -> that need's to create.
}

void MainWindow::on_actionRemove_RectBox_triggered()
{
    qDebug()<< ui->listView->currentIndex().data().toString();
    qDebug()<<count_list2;
    int counter = 1;

    QFile file(path+"/"+ui->listView->currentIndex().data().toString().split(".")[0]+".txt");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&file);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            qDebug()<< line;

            if(counter != count_list2)
                s.append(line + "\n");
            counter ++;
        }
        file.resize(0);
        t << s;
        file.close();
    }

    viewListImageview(ui->listView->currentIndex().data().toString());

    QString line;
    int line_count=0;
    QStringList list;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(&file);
        while (!stream.atEnd()){
            line = stream.readLine();
            line_count++;
            QStringList list1 = line.split(' ');

            int x = qRound((list1[1].toDouble() - list1[3].toDouble() / 2) * m_logo_pic.width());
            int y = qRound((list1[2].toDouble() - list1[4].toDouble() / 2) * m_logo_pic.height());
            int w = qRound((list1[1].toDouble() + list1[3].toDouble() / 2) * m_logo_pic.width());
            int h = qRound((list1[2].toDouble() + list1[4].toDouble() / 2) * m_logo_pic.height());

            QPainter painter(&m_logo_pic);
            QPen Red((Qt::black),8);
            painter.setPen(Red);

            painter.setFont(QFont("times",40,QFont::ExtraBold));

            painter.drawRect(x,y,w-x,h-y);
            painter.drawText(x, y-10, "Label "+ QString::number(line_count));
            ui->label->setPixmap(m_logo_pic.scaled(ui->label->size()));//Qt::KeepAspectRatio, Qt::SmoothTransformation));

            list<< "Label_"+ QString::number(line_count);
        }
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
        file.close();
    }
    else{
        list<<"Null";
        model1 = new QStringListModel(this);
        model1->setStringList(list);
        ui->listView_2->setModel(model1);
        file.close();
    }
}

// Rectangle box create
void MainWindow::on_actionCreate_Rectbox_triggered()
{
    qDebug() << "on_actionCreate_Rectbox_triggered";
    ui->label->setCursor(Qt::CrossCursor);

    connect(ui->label, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_Current_Pos()));
    connect(ui->label, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));
    connect(ui->label, SIGNAL(Mouse_Release()), this, SLOT(Mouse_Release()));
}

void MainWindow::on_actionSave_RectBox_triggered()
{

    qDebug() << result;
    QFile file(path+"/"+ui->listView->currentIndex().data().toString().split(".")[0]+".txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;

    QTextStream out(&file);

    out << result;
    file.close();

    result.clear();

    updatetxt(path+"/"+ui->listView->currentIndex().data().toString().split(".")[0]);
}


void MainWindow::on_actionAI_triggered()
{

}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{

}


void MainWindow::on_actionAuto_Train_triggered()
{

}

