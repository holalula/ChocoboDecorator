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
#include <QUrl>
#include <QDesktopServices>
ChocoboFFXIV::ChocoboFFXIV(QWidget *parent)
    : QMainWindow(parent)
{
	if (!IsRunasAdmin()) {
		QMessageBox::warning(this, "WARNING", QStringLiteral("请以管理员身份运行！"));
		exit(0);
	}

    ui.setupUi(this);
	
	sj_inject = 0;

	if (RegisterHotKey(HWND(this->winId()), 1, MOD_CONTROL | MOD_NOREPEAT, VK_F12)) {

	}
	if (RegisterHotKey(HWND(this->winId()), 2, MOD_CONTROL | MOD_NOREPEAT, VK_F11)) {

	}

	fp.GetPidByName(L"最终幻想XIV");
	//fp.GetPidByName(L"????XIV"); //en_os
	fp.OpenProcessByPid();
	fp.GetBaseAddressByPid();
	sj = false;
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

	connect(ui.pb_copy, SIGNAL(clicked()), this, SLOT(pb_copy_on_clicked()));
	connect(ui.le_px, SIGNAL(textEdited()), this, SLOT(le_px_on_textEdited()));
	connect(ui.le_py, SIGNAL(textEdited()), this, SLOT(le_py_on_textEdited()));
	connect(ui.le_pz, SIGNAL(textEdited()), this, SLOT(le_pz_on_textEdited()));
	//
	connect(ui.cb_sj, SIGNAL(stateChanged(int)), this, SLOT(cb_sj_on_stateChanged(int)));
	connect(ui.le_px_sj, SIGNAL(editingFinished()), this, SLOT(le_px_sj_on_editingFinished()));
	connect(ui.le_py_sj, SIGNAL(editingFinished()), this, SLOT(le_py_sj_on_editingFinished()));
	connect(ui.le_pz_sj, SIGNAL(editingFinished()), this, SLOT(le_pz_sj_on_editingFinished()));

	connect(ui.pb_mx, SIGNAL(clicked()), this, SLOT(pb_mx_on_clicked()));
	connect(ui.pb_ax, SIGNAL(clicked()), this, SLOT(pb_ax_on_clicked()));
	connect(ui.pb_my, SIGNAL(clicked()), this, SLOT(pb_my_on_clicked()));
	connect(ui.pb_ay, SIGNAL(clicked()), this, SLOT(pb_ay_on_clicked()));
	connect(ui.pb_mz, SIGNAL(clicked()), this, SLOT(pb_mz_on_clicked()));
	connect(ui.pb_az, SIGNAL(clicked()), this, SLOT(pb_az_on_clicked()));



	connect(ui.pb_sj, SIGNAL(clicked()), this, SLOT(pb_sj_on_clicked()));
	connect(ui.pb_sj_2, SIGNAL(clicked()), this, SLOT(pb_sj_2_on_clicked()));

	connect(ui.cbox_sj, SIGNAL(currentIndexChanged(QString)), this, SLOT(cbox_sj_on_currentIndexChanged(QString)));

	}

	}
	else {
		ui.tlabel->setText(QStringLiteral("未启动FFXIV，请确保开启dx11并以管理员开启本程序"));
	}
	connect(ui.pb_export, SIGNAL(clicked()), this, SLOT(pb_export_on_clicked()));
	connect(ui.pb_import, SIGNAL(clicked()), this, SLOT(pb_import_on_clicked()));
	connect(ui.pb_iteminfo, SIGNAL(clicked()), this, SLOT(pb_iteminfo_on_clicked()));
	connect(ui.link_github, SIGNAL(clicked()), this, SLOT(on_link_github()));
	connect(ui.link_qqun, SIGNAL(clicked()), this, SLOT(on_link_qqun()));
}



void ChocoboFFXIV::cbox_sj_on_currentIndexChanged(QString index) {
	if (ui.cbox_sj->currentIndex() == 0) {
		sj_inject = 0;
	}
	else if (ui.cbox_sj->currentIndex() == 1) {
		sj_inject = 1;
	}
}

void ChocoboFFXIV::pb_sj_on_clicked() {
	if (sj) {
		vector<float> pos_v = fp.GetActiveItemPos();
		float xx = pos_v[0];
		float yy = pos_v[1];
		float zz = pos_v[2];
		ui.le_px_sj->setText(QString::number(xx, 'f', 3));
		ui.le_py_sj->setText(QString::number(yy, 'f', 3));
		ui.le_pz_sj->setText(QString::number(zz, 'f', 3));
		QString qsx = ui.le_px_sj->text();
		xx = qsx.toFloat();
		QString qsy = ui.le_py_sj->text();
		yy = qsy.toFloat();
		QString qsz = ui.le_pz_sj->text();
		zz = qsz.toFloat();
		if (sj_inject == 0) {
			fp.InjectSjWithPos(xx, yy, zz);
		}
		else if (sj_inject == 1) {
			fp.InjectSjWithPos3(xx, yy, zz);
		}
		
		ui.tlabel->setText(QStringLiteral("请调整坐标后，点击锁定坐标"));
	}
}

void ChocoboFFXIV::on_link_github() {
	QDesktopServices::openUrl(QUrl("https://github.com/holalula/ChocoboDecorator"));
}

void ChocoboFFXIV::on_link_qqun() {
	//QDesktopServices::openUrl(QUrl("https://jq.qq.com/?_wv=1027&k=qdSgubGF"));
	QDesktopServices::openUrl(QUrl("https://bbs.nga.cn/read.php?tid=22777919"));

}

ChocoboFFXIV::~ChocoboFFXIV() {
    UnregisterHotKey(HWND(this->winId()), 1);
	UnregisterHotKey(HWND(this->winId()), 2);
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
				ui.tlabel->setText(QStringLiteral("已按下ctrl+f12"));
				if (sj) {
					//
					float nx = ui.le_px_sj->text().toFloat();
					float ny = ui.le_py_sj->text().toFloat();
					float nz = ui.le_pz_sj->text().toFloat();
					if (sj_inject == 0) {
						fp.reInjectSj();
					}
					else if (sj_inject == 1) {
						fp.reInjectSj3();
					}
					fp.SetActiveItemX(nx);
					fp.SetActiveItemY(ny);
					fp.SetActiveItemZ(nz);
					//keybd_event(VK_ESCAPE, 0, 0, 0);
					//keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
					ui.tlabel->setText(QStringLiteral("请在游戏内按下esc。如果所有家具都吸到一起，点击锁定坐标然后重新打开关闭布置家具"));
				}
				else {
					if (sj_inject == 0) {
						fp.reInjectSj();
					}
					else if (sj_inject == 1) {
						fp.reInjectSj3();
					}
				}
				

			}
			else if (pMsg->wParam == 2) {
				ui.tlabel->setText(QStringLiteral("已按下ctrl+f11"));
				/*
				if (sj) {
					vector<float> pos_v = fp.GetActiveItemPos();
					float xx = pos_v[0];
					float yy = pos_v[1];
					float zz = pos_v[2];
					ui.le_px_sj->setText(QString::number(xx, 'f', 3));
					ui.le_py_sj->setText(QString::number(yy, 'f', 3));
					ui.le_pz_sj->setText(QString::number(zz, 'f', 3));
					QString qsx = ui.le_px_sj->text();
					xx = qsx.toFloat();
					QString qsy = ui.le_py_sj->text();
					yy = qsy.toFloat();
					QString qsz = ui.le_pz_sj->text();
					zz = qsz.toFloat();
					fp.InjectSjWithPos(xx,yy,zz);
				}*/
			}
		}
	}
	return QWidget::nativeEvent(eventType, message, result);

}

void ChocoboFFXIV::pb_sj_2_on_clicked() {
	if (sj) {
		//
		float nx = ui.le_px_sj->text().toFloat();
		float ny = ui.le_py_sj->text().toFloat();
		float nz = ui.le_pz_sj->text().toFloat();
		if (sj_inject == 0) {
			fp.reInjectSj();
		}
		else if (sj_inject == 1) {
			fp.reInjectSj3();
		}
		
		fp.SetActiveItemX(nx);
		fp.SetActiveItemY(ny);
		fp.SetActiveItemZ(nz);
		//keybd_event(VK_ESCAPE, 0, 0, 0);
		//keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
		ui.tlabel->setText(QStringLiteral("请在游戏内按下esc。如果所有家具都吸到一起，点击锁定坐标然后重新打开关闭布置家具"));
	}
	else {
		if (sj_inject == 0) {
			fp.reInjectSj();
		}
		else if (sj_inject == 1) {
			fp.reInjectSj3();
		}
	}
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
		QString title = QStringLiteral("禁止在庭院开启");
		qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
		int tt = qrand()%3; //02
		QString info;
		info = QStringLiteral("回到室内打开布置家具即可解除警告");
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

void ChocoboFFXIV::pb_iteminfo_on_clicked() {
	ITEMINFO* iteminfoWindow = new ITEMINFO(Q_NULLPTR);
	iteminfoWindow->setWindowFlags(Qt::WindowStaysOnTopHint);
	iteminfoWindow->show();
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

void ChocoboFFXIV::cb_sj_on_stateChanged(int a) {
	if (a==Qt::Checked) {
		sj = true;
		ui.tlabel->setText(QStringLiteral("请在游戏内点击家具，然后点击\"获取当前家具坐标\""));
	}
	else {

	}
}
void ChocoboFFXIV::le_px_sj_on_editingFinished() {

}
void ChocoboFFXIV::le_py_sj_on_editingFinished() {

}
void ChocoboFFXIV::le_pz_sj_on_editingFinished() {

}
//modify pos
void ChocoboFFXIV::pb_mx_on_clicked() {
	QString qs = ui.le_delta->text();
	float delta_ = qs.toFloat();
	QString qs2 = ui.le_px_sj->text();
	float ox_ = qs2.toFloat();
	float res_ = ox_ - delta_;
	ui.le_px_sj->setText(QString::number(res_));
	//
	QString qsx = ui.le_px_sj->text();
	float xx = qsx.toFloat();
	QString qsy = ui.le_py_sj->text();
	float yy = qsy.toFloat();
	QString qsz = ui.le_pz_sj->text();
	float zz = qsz.toFloat();
	if (sj_inject == 0) {
		fp.InjectSjWithPos(xx, yy, zz);
	}
	else if (sj_inject == 1) {
		fp.InjectSjWithPos3(xx, yy, zz);
	}
	
}
void ChocoboFFXIV::pb_ax_on_clicked() {
	QString qs = ui.le_delta->text();
	float delta_ = qs.toFloat();
	QString qs2 = ui.le_px_sj->text();
	float ox_ = qs2.toFloat();
	float res_ = ox_ + delta_;
	ui.le_px_sj->setText(QString::number(res_));
	//
	QString qsx = ui.le_px_sj->text();
	float xx = qsx.toFloat();
	QString qsy = ui.le_py_sj->text();
	float yy = qsy.toFloat();
	QString qsz = ui.le_pz_sj->text();
	float zz = qsz.toFloat();
	if (sj_inject == 0) {
		fp.InjectSjWithPos(xx, yy, zz);
	}
	else if (sj_inject == 1) {
		fp.InjectSjWithPos3(xx, yy, zz);
	}
}
void ChocoboFFXIV::pb_my_on_clicked() {
	QString qs = ui.le_delta->text();
	float delta_ = qs.toFloat();
	QString qs2 = ui.le_py_sj->text();
	float ox_ = qs2.toFloat();
	float res_ = ox_ - delta_;
	ui.le_py_sj->setText(QString::number(res_));
	//
	QString qsx = ui.le_px_sj->text();
	float xx = qsx.toFloat();
	QString qsy = ui.le_py_sj->text();
	float yy = qsy.toFloat();
	QString qsz = ui.le_pz_sj->text();
	float zz = qsz.toFloat();
	if (sj_inject == 0) {
		fp.InjectSjWithPos(xx, yy, zz);
	}
	else if (sj_inject == 1) {
		fp.InjectSjWithPos3(xx, yy, zz);
	}
	
}
void ChocoboFFXIV::pb_ay_on_clicked() {
	QString qs = ui.le_delta->text();
	float delta_ = qs.toFloat();
	QString qs2 = ui.le_py_sj->text();
	float ox_ = qs2.toFloat();
	float res_ = ox_ + delta_;
	ui.le_py_sj->setText(QString::number(res_));
	//
	QString qsx = ui.le_px_sj->text();
	float xx = qsx.toFloat();
	QString qsy = ui.le_py_sj->text();
	float yy = qsy.toFloat();
	QString qsz = ui.le_pz_sj->text();
	float zz = qsz.toFloat();
	if (sj_inject == 0) {
		fp.InjectSjWithPos(xx, yy, zz);
	}
	else if (sj_inject == 1) {
		fp.InjectSjWithPos3(xx, yy, zz);
	}
}
void ChocoboFFXIV::pb_mz_on_clicked() {
	QString qs = ui.le_delta->text();
	float delta_ = qs.toFloat();
	QString qs2 = ui.le_pz_sj->text();
	float ox_ = qs2.toFloat();
	float res_ = ox_ - delta_;
	ui.le_pz_sj->setText(QString::number(res_));
	//
	QString qsx = ui.le_px_sj->text();
	float xx = qsx.toFloat();
	QString qsy = ui.le_py_sj->text();
	float yy = qsy.toFloat();
	QString qsz = ui.le_pz_sj->text();
	float zz = qsz.toFloat();
	if (sj_inject == 0) {
		fp.InjectSjWithPos(xx, yy, zz);
	}
	else if (sj_inject == 1) {
		fp.InjectSjWithPos3(xx, yy, zz);
	}
}
void ChocoboFFXIV::pb_az_on_clicked() {
	QString qs = ui.le_delta->text();
	float delta_ = qs.toFloat();
	QString qs2 = ui.le_pz_sj->text();
	float ox_ = qs2.toFloat();
	float res_ = ox_ + delta_;
	ui.le_pz_sj->setText(QString::number(res_));
	//
	QString qsx = ui.le_px_sj->text();
	float xx = qsx.toFloat();
	QString qsy = ui.le_py_sj->text();
	float yy = qsy.toFloat();
	QString qsz = ui.le_pz_sj->text();
	float zz = qsz.toFloat();
	if (sj_inject == 0) {
		fp.InjectSjWithPos(xx, yy, zz);
	}
	else if (sj_inject == 1) {
		fp.InjectSjWithPos3(xx, yy, zz);
	}
}