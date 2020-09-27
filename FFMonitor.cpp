#include "FFMonitor.h"
#include <QMessageBox>
#include <QDir>
#include <tchar.h>

char* TCHAR2char(const TCHAR* STR)
{
	//返回字符串的长度
	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, FALSE);
	//申请一个多字节的字符串变量
	char* str = new char[sizeof(char) * size];
	//将STR转成str
	WideCharToMultiByte(CP_ACP, 0, STR, -1, str, size, NULL, FALSE);
	return str;
}

FFMonitor::FFMonitor(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pb_memo, SIGNAL(clicked()), this, SLOT(pb_memo_on_clicked()));
	connect(ui.pb_net, SIGNAL(clicked()), this, SLOT(pb_net_on_clicked()));

	connect(ui.pb_getpath, SIGNAL(clicked()), this, SLOT(pb_getpath_on_clicked()));
	connect(ui.pb_injectff, SIGNAL(clicked()), this, SLOT(pb_injectff_on_clicked()));
	connect(ui.pb_loaddll, SIGNAL(clicked()), this, SLOT(pb_loaddll_on_clicked()));
	connect(ui.pb_check, SIGNAL(clicked()), this, SLOT(pb_check_on_clicked()));

	connect(ui.pb_getinit, SIGNAL(clicked()), this, SLOT(pb_getinit_on_clicked()));
	connect(ui.pb_setinit, SIGNAL(clicked()), this, SLOT(pb_setinit_on_clicked()));
	connect(ui.pb_getmode, SIGNAL(clicked()), this, SLOT(pb_getmode_on_clicked()));
	connect(ui.pb_setmode, SIGNAL(clicked()), this, SLOT(pb_setmode_on_clicked()));

	connect(ui.pb_getx, SIGNAL(clicked()), this, SLOT(pb_getx_on_clicked()));
	connect(ui.pb_gety, SIGNAL(clicked()), this, SLOT(pb_gety_on_clicked()));
	connect(ui.pb_getz, SIGNAL(clicked()), this, SLOT(pb_getz_on_clicked()));
	connect(ui.pb_getr, SIGNAL(clicked()), this, SLOT(pb_getr_on_clicked()));
	connect(ui.pb_setx, SIGNAL(clicked()), this, SLOT(pb_setx_on_clicked()));
	connect(ui.pb_sety, SIGNAL(clicked()), this, SLOT(pb_sety_on_clicked()));
	connect(ui.pb_setz, SIGNAL(clicked()), this, SLOT(pb_setz_on_clicked()));
	connect(ui.pb_setr, SIGNAL(clicked()), this, SLOT(pb_setr_on_clicked()));

	model = new QStandardItemModel(this);
	modelclear();
	ui.tableView->setModel(model);
	citemInfo.update();


}
FFMonitor::~FFMonitor()
{
}
void FFMonitor::modelclear() {
	model->clear();
	model->setColumnCount(6);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("X"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("Y"));
	model->setHeaderData(4, Qt::Horizontal, QStringLiteral("Z"));
	model->setHeaderData(5, Qt::Horizontal, QStringLiteral("R"));

}
void FFMonitor::SetFp(FFProcess* t) {
	fp = *t;
}

void FFMonitor::pb_memo_on_clicked() {
	fp.GetvFurins();
	int _check = fp.CheckPermis();
	if (!_check) {
		QMessageBox::warning(this, QStringLiteral("权限错误"), QStringLiteral("请确保已打开布置家具面板"));
		return;
	}
	//fp.ins2cat();
	//fp.categoryList;
	//tableview
	modelclear();
	int crow = 0;
	for (int i = 0; i < fp.vfurins.size(); i++) {
		int category = fp.vfurins[i].cate;
		//int count = fp.categoryList[i].count;
			QList<QStandardItem*> temp_list;

			if (citemInfo.id2name2.find(category) == citemInfo.id2name2.end()) {
				temp_list << new QStandardItem(QStringLiteral("未知家具"));
			}
			else {
				string cstr = citemInfo.id2name2[category];
				temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
			}
			temp_list << new QStandardItem(QString::number(category, 10));
			temp_list << new QStandardItem(QString::number(fp.vfurins[i].x));
			temp_list << new QStandardItem(QString::number(fp.vfurins[i].y));
			temp_list << new QStandardItem(QString::number(fp.vfurins[i].z));
			temp_list << new QStandardItem(QString::number(fp.vfurins[i].r));
			model->insertRow(crow++, temp_list);
		

	}
}

void FFMonitor::pb_net_on_clicked() {
	int _len = _ffhook.get_send_len();
	ui.label->setText(QString::number(_len));
	//char _buf[1024];
	//memcpy(_buf, _ffhook.get_send_buf, _len);
	//ui.textEdit->setText(_buf);
	//
	QString fileDir = QCoreApplication::applicationDirPath();
	fileDir += "\\list\\";
	QString fileName("ffnet.log");
	QDir dir(fileDir);
	wcscpy(_ffhook.logPath, reinterpret_cast<const wchar_t*>(dir.absoluteFilePath(fileName).utf16()));
	FILE* pLogFile;
	pLogFile = fopen(TCHAR2char(_ffhook.logPath), "r+");
	char _buf[512];
	fread(_buf, sizeof(char), _len, pLogFile);

	QByteArray _byte = QByteArray(_buf,_len);

	ui.textEdit->setText(_byte.toHex());
	//char _a =
	char _t[4] = { _byte.toHex().data()[160],_byte.toHex().data()[161],_byte.toHex().data()[162],_byte.toHex().data()[163] };
	ui.label->setText(QString(_t));
}

void FFMonitor::pb_getpath_on_clicked() {
	QString fileDir = QCoreApplication::applicationDirPath();
	fileDir += "\\list\\";
	QString fileName("ffnetwork.dll");
	QDir dir(fileDir);
	FFHook& ffHook = FFHook::get_instance();
	wcscpy(ffHook.dllPath, reinterpret_cast<const wchar_t*>(dir.absoluteFilePath(fileName).utf16()));
	ui.label->setText(QString::fromWCharArray(ffHook.dllPath));
}
void FFMonitor::pb_injectff_on_clicked() {
	_ffhook._In(_ffhook.dllPath, _ffhook.ProcessFind(_T("ffxiv_dx11.exe")));
	ui.label->setText(QStringLiteral("已注入ffxiv_dx11.exe"));
}
void FFMonitor::pb_loaddll_on_clicked() {
	if (1 == _ffhook._LoadDLLGetFP()) {
		ui.label->setText(QStringLiteral("已load"));
	}

}
void FFMonitor::pb_check_on_clicked() {
	if (TRUE == _ffhook.CheckDllInProcess(_ffhook.ProcessFind(_T("ffxiv_dx11.exe")), _ffhook.dllPath)) {
		ui.label->setText(QStringLiteral("检测到"));
	}
	else {
		ui.label->setText("0");
	}
}

void FFMonitor::pb_getinit_on_clicked() {
	ui.lineEdit->setText(QString::number(_ffhook.getInit()));
}
void FFMonitor::pb_setinit_on_clicked() {
	_ffhook.setInit(ui.lineEdit->text().toInt());
}

void FFMonitor::pb_getmode_on_clicked() {
	ui.lineEdit_2->setText(QString::number(_ffhook.getMode()));
}
void FFMonitor::pb_setmode_on_clicked() {
	_ffhook.setMode(ui.lineEdit_2->text().toInt());
}


void FFMonitor::pb_getx_on_clicked() {
	ui.lineEdit_3->setText(QString::number(_ffhook.getX()));
}
void FFMonitor::pb_setx_on_clicked() {
	_ffhook.setX(ui.lineEdit_3->text().toFloat());
}
void FFMonitor::pb_gety_on_clicked() {
	ui.lineEdit_4->setText(QString::number(_ffhook.getY()));
}
void FFMonitor::pb_sety_on_clicked() {
	_ffhook.setY(ui.lineEdit_4->text().toFloat());
}
void FFMonitor::pb_getz_on_clicked() {
	ui.lineEdit_5->setText(QString::number(_ffhook.getZ()));
}
void FFMonitor::pb_setz_on_clicked() {
	_ffhook.setZ(ui.lineEdit_5->text().toFloat());
}
void FFMonitor::pb_getr_on_clicked() {
	ui.lineEdit_6->setText(QString::number(_ffhook.getR()));
}
void FFMonitor::pb_setr_on_clicked() {
	_ffhook.setR(ui.lineEdit_6->text().toFloat());
}