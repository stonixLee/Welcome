#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QFileDialog>
#include    <QMessageBox>
#include    <QUuid>
void MainWindow::initUI()
{
    setSysCombobox();

}

void MainWindow::setSysCombobox()
{
    QSqlQuery ProjectName(DB);
    ProjectName.exec("select Project_Name from t_simulationproject");
    while(ProjectName.next())
    {
        ui->HypotaxisCombobox->addItem(ProjectName.value(0).toString());
    }
    ProName = ui->HypotaxisCombobox->currentText();
}

void MainWindow::initTree()
{

}

void MainWindow::DatabaseConnecting()
{
    DB =QSqlDatabase::addDatabase("QMYSQL");
    DB.setDatabaseName("Simu");
    DB.setHostName("localhost");
    DB.setPassword("root");
    DB.setPort(3306);
    DB.setUserName("root");
    if (!DB.open())   //打开数据库
    {
        QMessageBox::warning(this, "错误", "打开数据库失败",
                                 QMessageBox::Ok,QMessageBox::NoButton);
        return;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DatabaseConnecting();
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Search_clicked()
{

}

void MainWindow::on_SearchMF_Btn_clicked()
{
    MFPath = QFileDialog::getOpenFileName(this,"选择模型文件","","所有文件(*.*)");
    if(MFPath.isEmpty())
        return;
    ui->MF_Edit->setText(MFPath);

}

void MainWindow::on_XML_Btn_clicked()
{
    XMLfullName = QFileDialog::getOpenFileName(this,"选择XML文件","","所有文件(*.*);;XML文件(*.xml)");
    if(XMLfullName.isEmpty())
        return;
    ui->XML_Edit->setText(XMLfullName);
}


void MainWindow::on_ModelSubmitBtn_clicked()//上传到数据库
{
    QString MF = getFinalFolderName(MFPath);
    QString XML = getFinalFolderName(XMLfullName);
    QString modelID = QUuid::createUuid().toString().mid(1,UUID_LENGTH);
    //将MF,XML,modelID,aDir,version信息写进数据库


        if(!isContainItem())
        {
            ui->HypotaxisCombobox->addItem(ProName);//在下拉列表中添加子系统
            QSqlQuery addProject(DB);
            addProject.prepare("insert into t_simulationproject(Project_id,Project_Number,"
                                "Project_Name,Project_Modelfile) values(?,?,?,?)");
            addProject.addBindValue(QUuid::createUuid().toString().mid(1,UUID_LENGTH));
            addProject.addBindValue("1");
            addProject.addBindValue(ProName);
            addProject.addBindValue("文件一");//要进行修改*********************
            qDebug()<<QUuid::createUuid().toString().mid(1,UUID_LENGTH)<<ProName;
            addProject.exec();
        }
}



QString MainWindow::getFinalFolderName(const QString &fullPathName)//从一个完整的目录名称里获得最后的文件名称
{
    int cnt=fullPathName.length();
    int i = fullPathName.lastIndexOf("/");
    QString str = fullPathName.right(cnt-1-i);
    return str;
}

bool MainWindow::isContainItem()//判断是否在所属系统的下拉列表中含有ProName
{
    int num = ui->HypotaxisCombobox->count();
    qDebug()<<num;
    int i;
    for(i=0;i<num;i++)
        if(ui->HypotaxisCombobox->itemText(i)==ProName)
          {
            break;
         }

    qDebug()<<i;
    return i==num ? false:true;

}

void MainWindow::on_ModelNameEdit_cursorPositionChanged(int arg1, int arg2)
{
    modelName = ui->ModelNameEdit->text();
}

void MainWindow::on_HypotaxisCombobox_currentIndexChanged(const QString &arg1)
{
    ProName=ui->HypotaxisCombobox->currentText();
}

void MainWindow::on_MF_PosBtn_clicked()//还需完善:先选择服务器ip再选择存放位置
{
    QString assignPath = "D:/软件和资料/数据库模型/modelfile";
    aDir = QFileDialog::getExistingDirectory(this,"选择一个目录",assignPath,QFileDialog::ShowDirsOnly);
    ui->MF_SubmitPosEdit->setText(aDir);

}

void MainWindow::on_ModelVersionEdit_editingFinished()
{
    version=ui->ModelVersionEdit->text();
}

void MainWindow::on_HypotaxisCombobox_editTextChanged(const QString &arg1)
{
    ProName = ui->HypotaxisCombobox->currentText();
}
