#include "ChocoboFFXIV.h"
#include "ui_ChocoboFFXIV.h"
#include <cstdlib>
#include "QDoubleValidator"
#include "EXPORT.h"
#include "IMPORT.h"
#include "ITEMINFO.h"
#include <QTimer>
#include <QTime>
#include <QMessageBox>

ChocoboFFXIV::ChocoboFFXIV(QWidget *parent)
    : QMainWindow(parent)
{
	if (!IsRunasAdmin()) {
		QMessageBox::warning(this, "WARNING", QStringLiteral("请以管理员身份运行！"));
		exit(0);
	}

    ui.setupUi(this);
	
	//if (RegisterHotKey(HWND(this->winId()), 1, MOD_CONTROL | MOD_NOREPEAT, VK_F12)) 

	fp.GetPidByName(L"最终幻想XIV");
	fp.OpenProcessByPid();
	fp.GetBaseAddressByPid();

	if(fp.GetPid()!=0){
		ui.tlabel->setText(QStringLiteral("已启动FFXIV"));

		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(updatePos()));
		timer2 = new QTimer(this);
		connect(timer2, SIGNAL(timeout()), this, SLOT(checkwhere()));
		timer2->start(500);
	{
	double pos_bottom = -100.0;
	double pos_top = 100.0;
	ui.le_px->setValidator(new QDoubleValidator(pos_bottom, pos_top, 3, this));
	ui.le_py->setValidator(new QDoubleValidator(pos_bottom, pos_top, 3, this));
	ui.le_pz->setValidator(new QDoubleValidator(pos_bottom, pos_top, 3, this));
	}
	{
	connect(ui.cb_placeanywhere, SIGNAL(stateChanged(int)), this, SLOT(cb_placeanywhere_on_stateChanged(int)));
	connect(ui.cb_activepos, SIGNAL(stateChanged(int)), this, SLOT(cb_activepos_on_stateChanged(int)));
	connect(ui.le_px, SIGNAL(editingFinished()), this, SLOT(le_px_on_editingFinished()));
	connect(ui.le_py, SIGNAL(editingFinished()), this, SLOT(le_py_on_editingFinished()));
	connect(ui.le_pz, SIGNAL(editingFinished()), this, SLOT(le_pz_on_editingFinished()));
	connect(ui.pb_export, SIGNAL(clicked()), this, SLOT(pb_export_on_clicked()));
	connect(ui.pb_import, SIGNAL(clicked()), this, SLOT(pb_import_on_clicked()));
	connect(ui.pb_copy, SIGNAL(clicked()), this, SLOT(pb_copy_on_clicked()));
	connect(ui.le_px, SIGNAL(textEdited()), this, SLOT(le_px_on_textEdited()));
	connect(ui.le_py, SIGNAL(textEdited()), this, SLOT(le_py_on_textEdited()));
	connect(ui.le_pz, SIGNAL(textEdited()), this, SLOT(le_pz_on_textEdited()));
	}

	}
	else {
		ui.tlabel->setText(QStringLiteral("未启动FFXIV，请确保开启dx11并以管理员开启本程序"));
	}
}

ChocoboFFXIV::~ChocoboFFXIV() {
    UnregisterHotKey(HWND(this->winId()), 1);
}


void ChocoboFFXIV::labelDebug(QString str) {
	
}


bool ChocoboFFXIV::nativeEvent(const QByteArray& eventType, void* message, long* result) {
	if (eventType == "windows_generic_MSG")
	{
		MSG* pMsg = reinterpret_cast<MSG*>(message);
		if (pMsg->message == WM_HOTKEY)
		{
			if (pMsg->wParam == 1) {
				//ui.tlabel->setText(QStringLiteral("已按下ctrl+f12"));
			}	
		}
	}
	return QWidget::nativeEvent(eventType, message, result);

}

void ChocoboFFXIV::cb_placeanywhere_on_stateChanged(int a) {
	if (a == Qt::Checked) {
		fp.InjectPlaceAnywhere();
	}
	else {
		fp.reInjectPlaceAnywhere();
	}
}

void ChocoboFFXIV::checkwhere() {
	int iw = fp.IsWarehouse();
	if (iw == 2) {
		ui.iswarehouse->setText(QStringLiteral("已布置"));
	}
	else if (iw == 1) {
		ui.iswarehouse->setText(QStringLiteral("仓库"));
	}
	else if (iw == -1) {
		ui.iswarehouse->setText(QStringLiteral("庭院"));
		QString title = "???????";
		qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
		int tt = qrand()%3; //02
		QString info;
		if (tt == 0) {
			 info = QStringLiteral("人女天下第一！");
		}
		else if (tt == 1) {
			 info = QStringLiteral("拉拉肥是什么东西啊！");
		}
		else if (tt == 2) {
			 info = QStringLiteral("猫娘爪巴！");
		}
		QMessageBox::critical(this, title, info);
		exit(0);
	}
	else {
		ui.iswarehouse->setText(QStringLiteral("未检测到_"));
	}
}

void ChocoboFFXIV::updatePos() {
	vector<float> pos = fp.GetActiveItemPos();
	float npx, npy, npz;
	npx = pos[0];
	npy = pos[1];
	npz = pos[2];
	ui.l_px->setText(QString::number(npx,'f',3));
	ui.l_py->setText(QString::number(npy, 'f', 3));
	ui.l_pz->setText(QString::number(npz, 'f', 3));	
}

void ChocoboFFXIV::cb_activepos_on_stateChanged(int a) {
	if (a == Qt::Checked) {
		vector<float> pos = fp.GetActiveItemPos();
		ui.le_px->setText(QString::number(pos[0], 'f', 3));
		ui.le_py->setText(QString::number(pos[1], 'f', 3));
		ui.le_pz->setText(QString::number(pos[2], 'f', 3));
		ui.l_px->setText(QString::number(pos[0], 'f', 3));
		ui.l_py->setText(QString::number(pos[1], 'f', 3));
		ui.l_pz->setText(QString::number(pos[2], 'f', 3));
		timer->start(200);
	}
	else {
		timer->stop();
	}
}

void ChocoboFFXIV::le_px_on_editingFinished() {
	fp.SetActiveItemX(ui.le_px->text().toFloat());
}

void ChocoboFFXIV::le_py_on_editingFinished() {
	fp.SetActiveItemY(ui.le_py->text().toFloat());
}

void ChocoboFFXIV::le_pz_on_editingFinished() {
	fp.SetActiveItemZ(ui.le_pz->text().toFloat());
}

void ChocoboFFXIV::pb_export_on_clicked() {
	EXPORT* ExportWindow = new EXPORT(Q_NULLPTR);
	ExportWindow->SetFp(&fp);
	ExportWindow->setWindowFlags(Qt::WindowStaysOnTopHint);
	ExportWindow->show();
}

void ChocoboFFXIV::pb_import_on_clicked() {
	IMPORT* ImportWindow = new IMPORT(Q_NULLPTR);
	ImportWindow->SetFp(&fp);
	ImportWindow->setWindowFlags(Qt::WindowStaysOnTopHint);
	ImportWindow->show();
}



void ChocoboFFXIV::pb_copy_on_clicked(){
	//ui.l_px.	
	ui.le_px->setText(ui.l_px->text());
	ui.le_py->setText(ui.l_py->text());
	ui.le_pz->setText(ui.l_pz->text());

}

BOOL ChocoboFFXIV::IsRunasAdmin()
{
	BOOL bElevated = FALSE;
	HANDLE hToken = NULL;

	// Get current process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		return FALSE;

	TOKEN_ELEVATION tokenEle;
	DWORD dwRetLen = 0;

	// Retrieve token elevation information
	if (GetTokenInformation(hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen))
	{
		if (dwRetLen == sizeof(tokenEle))
		{
			bElevated = tokenEle.TokenIsElevated;
		}
	}

	CloseHandle(hToken);
	return bElevated;
}

/*
void ChocoboFFXIV::le_px_on_textEdited() {

}
void ChocoboFFXIV::le_py_on_textEdited() {

}
void ChocoboFFXIV::le_pz_on_textEdited() {

}//textEdited
*/