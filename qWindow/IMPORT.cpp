#include "IMPORT.h"
#include "FFHook.h"
#include "ItemList.h"
#include <io.h>
#include <qDebug>
#include <QMessageBox>
#include <tchar.h>
#include <TlHelp32.h>
#include <process.h>
#include <qdir.h>

DWORD ProcessFind(LPCTSTR Exename) {
	HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (!hp) {
		return false;
	}
	PROCESSENTRY32 info;
	info.dwSize = sizeof(info);
	if (!Process32First(hp, &info))
		return false;
	while (1) {
		if (_tcscmp(info.szExeFile, Exename) == 0) {
			return info.th32ProcessID;
		}
		if (!Process32Next(hp, &info)) {
			return false;
		}
	}
	return false;
}


IMPORT::IMPORT(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	if (RegisterHotKey(HWND(this->winId()), 1, MOD_CONTROL | MOD_NOREPEAT, VK_F12)) {

	}
	ui.tableView->setMouseTracking(true);
	model = new QStandardItemModel(this);

	model->setColumnCount(6);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("X"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("Y"));
	model->setHeaderData(4, Qt::Horizontal, QStringLiteral("Z"));
	model->setHeaderData(5, Qt::Horizontal, QStringLiteral("R"));

	
	connect(ui.checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(cb_checkBox_2_on_stateChanged(int)));
	connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(cb_checkBox_on_stateChanged(int)));
	connect(ui.cb_iblue, SIGNAL(stateChanged(int)), this, SLOT(cb_iblue_on_stateChanged(int)));
	connect(ui.pb_1_1, SIGNAL(clicked()), this, SLOT(pb_1_1_on_clicked()));
	connect(ui.pb_1, SIGNAL(clicked()), this, SLOT(pb_1_on_clicked()));
	connect(ui.pb_2, SIGNAL(clicked()), this, SLOT(pb_2_on_clicked()));
	connect(ui.pb_3, SIGNAL(clicked()), this, SLOT(pb_3_on_clicked()));

	connect(ui.pb_inject, SIGNAL(clicked()), this, SLOT(pb_inject_on_clicked()));
	connect(ui.pb_einject, SIGNAL(clicked()), this, SLOT(pb_einject_on_clicked()));
	connect(ui.pb_dllbf, SIGNAL(clicked()), this, SLOT(pb_dllbf_on_clicked()));

	ui.tableView->setModel(model);

	FFJson j_r;
	j_r.ReadJsonFile("delaytime.json");
	s_t2 = j_r.document["s_t2"].GetInt();
	s_t0 = j_r.document["s_t0"].GetInt();
	s_tn = j_r.document["s_tn"].GetInt();

	this->citemInfo.update();


}
IMPORT::~IMPORT()
{
	UnregisterHotKey(HWND(this->winId()), 1);
}
void IMPORT::modelclear() {
	model->clear();
	model->setColumnCount(6);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("X"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("Y"));
	model->setHeaderData(4, Qt::Horizontal, QStringLiteral("Z"));
	model->setHeaderData(5, Qt::Horizontal, QStringLiteral("R"));
}
void IMPORT::SetFp(FFProcess* t) {
	fp = *t;
}
void IMPORT::resizeEvent(QResizeEvent* event)
{
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	void on_tableView_entered(const QModelIndex & index);

}
void IMPORT::on_tableView_entered(const QModelIndex& index)
{
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}
bool IMPORT::nativeEvent(const QByteArray& eventType, void* message, long* result) {
	if (eventType == "windows_generic_MSG")
	{
		MSG* pMsg = reinterpret_cast<MSG*>(message);
		if (pMsg->message == WM_HOTKEY)
		{
			if (pMsg->wParam == 1) {
				//->setText();
				//setWindowTitle(QStringLiteral("已按下ctrl+f12"));
				int click_n = ui.le_click->text().toInt();
				for (int i = 0; i < click_n; i++) {
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					Sleep(500);
				}

			}
			//else if (pMsg->wParam == 2) {
				//ui.tlabel->setText(QStringLiteral("已按下ctrl+f11"));
			//}
		}
	}
	return QWidget::nativeEvent(eventType, message, result);

}




void IMPORT::cb_iblue_on_stateChanged(int a) {
	if (a == Qt::Checked) {
		fp.InjectBlue();
	}
	else {
		fp.reInjectBlue();
	}
}
void IMPORT::cb_checkBox_on_stateChanged(int a) {
	if (a == Qt::Checked){
	vector<int> il = fp.GetItemList();
	modelclear();
	int crow = 0;
	for (int i = 0; i < il.size(); i++) {
		QList<QStandardItem*> temp_list;
		if (citemInfo.id2name.find(il[i]) == citemInfo.id2name.end()) {
			temp_list << new QStandardItem(QStringLiteral("未知家具"));
		}
		else {
			string cstr = citemInfo.id2name[il[i]];
			temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
		}
		temp_list << new QStandardItem(QString::number(il[i]));
		model->insertRow(crow++, temp_list);
	}
}
}
void IMPORT::cb_checkBox_2_on_stateChanged(int a) {
	if (a == Qt::Checked) {
		fp.GetvFurins();
		int _check = fp.CheckPermis();
		if (!_check) {
			QMessageBox::warning(this, QStringLiteral("权限错误"), QStringLiteral("请确保已打开布置家具面板,家具数量变动后请重新进入房间"));
			return;
		}
		fp.ins2cat();
		//fp.categoryList;
		//tableview
		modelclear();
		int samecategoryIndex = 1;
		int crow = 0;
		for (int i = 0; i < fp.categoryList.size(); i++) {
			int category = fp.categoryList[i].categoryId;
			int count = fp.categoryList[i].count;
			for (int j = 0; j < count; j++) {
				QList<QStandardItem*> temp_list;

				if (citemInfo.id2name2.find(category) == citemInfo.id2name2.end()) {
					temp_list << new QStandardItem(QStringLiteral("未知家具"));
				}
				else {
					string cstr = citemInfo.id2name2[category];
					temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
				}
				temp_list << new QStandardItem(QString::number(category, 10));
				temp_list << new QStandardItem(QString::number(j + 1));
				temp_list << new QStandardItem(QString::number(fp.categoryList[i].posX[j]));
				temp_list << new QStandardItem(QString::number(fp.categoryList[i].posY[j]));
				temp_list << new QStandardItem(QString::number(fp.categoryList[i].posZ[j]));
				temp_list << new QStandardItem(QString::number(fp.categoryList[i].r[j]));
				model->insertRow(crow++, temp_list);
			}

		}
	}
	else {

	}
}



void IMPORT::pb_inject_on_clicked() {
	if (!b_cmp) {
		QMessageBox::information(NULL, QStringLiteral("操作警告"), QStringLiteral("请确认匹配成功后再注入"));
		return;
	}
	QString fileDir = QCoreApplication::applicationDirPath();
	fileDir+="\\list\\";
	QString fileName("ffnetwork.dll");
	QDir dir(fileDir);
	qDebug() << dir.absoluteFilePath(fileName) << endl;
	FFHook& ffHook = FFHook::get_instance();
	wcscpy(ffHook.dllPath, reinterpret_cast<const wchar_t*>(dir.absoluteFilePath(fileName).utf16()));
	ffHook._LoadDLLGetFP();
	ffHook._In(ffHook.dllPath, ffHook.ProcessFind(_T("ffxiv_dx11.exe"))); //suc
}
void IMPORT::pb_einject_on_clicked() {
	QString fileDir = QCoreApplication::applicationDirPath();
	QString fileName("ffnetwork.dll");
	QDir dir(fileDir);
	qDebug() << dir.absoluteFilePath(fileName).toStdWString().c_str() << endl;
	fp.EIn(reinterpret_cast<const wchar_t*>(dir.absoluteFilePath(fileName).utf16()), ProcessFind(_T("ffxiv_dx11.exe")));
	FFHook& ffHook = FFHook::get_instance();
	ffHook._FreeDLL();
	//fp.EIn(dir.absoluteFilePath(fileName).toStdWString().c_str(), ProcessFind(_T("ffxiv_dx11.exe")));
}
void IMPORT::pb_1_1_on_clicked(){
	QString s1 = ui.le_import_2->text();
	QString ffp = ".\\list\\" + s1;
	QByteArray ba2;
	ba2.append(ffp);
	const char* c2 = ba2.data();
	//
	if ((_access(c2, 0)) == -1) {
		QMessageBox::warning(this, QStringLiteral("文件不存在"), QStringLiteral("文件不存在，请检查文件名"));
		return;
	}
	//categoryList1 categoryList2
	fp.GetvFurins();
	int _check = fp.CheckPermis();
	if (!_check) {
		QMessageBox::warning(this, QStringLiteral("权限错误"), QStringLiteral("请确保已打开布置家具面板"));
		return;
	}
	fp.ins2cat();
	//tableview
	modelclear();
	int samecategoryIndex = 1;
	int crow = 0;
	for (int i = 0; i < fp.categoryList.size(); i++) {
		int category = fp.categoryList[i].categoryId;
		int count = fp.categoryList[i].count;
		for (int j = 0; j < count; j++) {
			QList<QStandardItem*> temp_list;
			if (citemInfo.id2name2.find(category) == citemInfo.id2name2.end()) {
				temp_list << new QStandardItem(QStringLiteral("未知家具"));
			}
			else {
				string cstr = citemInfo.id2name2[category];
				temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
			}
			temp_list << new QStandardItem(QString::number(category, 10));
			temp_list << new QStandardItem(QString::number(j + 1));
			temp_list << new QStandardItem(QString::number(fp.categoryList[i].posX[j]));
			temp_list << new QStandardItem(QString::number(fp.categoryList[i].posY[j]));
			temp_list << new QStandardItem(QString::number(fp.categoryList[i].posZ[j]));
			temp_list << new QStandardItem(QString::number(fp.categoryList[i].r[j]));
			model->insertRow(crow++, temp_list);
		}

	}
	vector<int> _temp_cmp;
	for (int i = 0; i < fp.categoryList.size(); i++) {
		for (int j = 0; j < fp.categoryList[i].count; j++) {
			_temp_cmp.push_back(fp.categoryList[i].categoryId);
		}
	}

	//compare
	ItemList t_itemList;
	vector<ItemList::CategoryInstance> _itemList = t_itemList.ReadCateListJson(c2);
	int err_a = -1, err_b = -1, err_c = -1, err_d = -1;
	int res = t_itemList.Compare(_temp_cmp, _itemList, err_a, err_b, err_c, err_d);
	if (res == 1) {
		this->setWindowTitle("1");
		b_cmp = true;
	}
	else {
		this->setWindowTitle("0");
		b_cmp = false;
	}
	cout << "qt " << err_a << " " << err_b << " " << err_c << " " << err_d << endl;
	if (err_a == 1) {
		//err b=tid  c=tcount d=json_count
		QString title = QStringLiteral("数量不匹配:id=") + QString::number(err_b);
		QString itemname;
		if (citemInfo.id2name2.find(err_b) == citemInfo.id2name2.end()) {
			itemname = QStringLiteral("未知家具");
		}
		else {
			string cstr = citemInfo.id2name2[err_b];
			itemname = QString::fromLocal8Bit(cstr.c_str());
		}
		QString qstr = QStringLiteral("家具：") + QString::number(err_b) + QStringLiteral("(") + itemname + QStringLiteral(")")
			+ QStringLiteral("在当前家具列表中数量为") + QString::number(err_c) + QStringLiteral(",但在匹配文件中为") + QString::number(err_d);
		QMessageBox::warning(this, title, qstr);
	}
	else if (err_a == 2) {
		//err b=tid not found in JSON
		QString title = QStringLiteral("种类不匹配:id=") + QString::number(err_b);
		QString itemname;
		if (citemInfo.id2name2.find(err_b) == citemInfo.id2name2.end()) {
			itemname = QStringLiteral("未知家具");
		}
		else {
			string cstr = citemInfo.id2name2[err_b];
			itemname = QString::fromLocal8Bit(cstr.c_str());
		}
		QString qstr = QStringLiteral("家具：") + QString::number(err_b) + QStringLiteral("(") + itemname + QStringLiteral(")")
			+ QStringLiteral("出现在当前家具列表中，但未出现在匹配文件中");
		QMessageBox::warning(this, title, qstr);
	}
	else if (err_a == 3) {
		//err b=cid not found in your house
		QString title = QStringLiteral("种类不匹配:id=") + QString::number(err_b);
		QString itemname;
		if (citemInfo.id2name2.find(err_b) == citemInfo.id2name2.end()) {
			itemname = QStringLiteral("未知家具");
		}
		else {
			string cstr = citemInfo.id2name2[err_b];
			itemname = QString::fromLocal8Bit(cstr.c_str());
		}
		QString qstr = QStringLiteral("家具：") + QString::number(err_b) + QStringLiteral("(") + itemname + QStringLiteral(")")
			+ QStringLiteral("出现在匹配文件中，但未出现在当前家具列表中");
		QMessageBox::warning(this, title, qstr);
	}

}

void IMPORT::pb_1_on_clicked() {
	QString s1 = ui.le_import->text();
	QString ffp = ".\\list\\" + s1;
	QByteArray ba2;
	ba2.append(ffp);     
	const char* c2 = ba2.data();

	//
	if ((_access(c2, 0)) == -1) {
		QMessageBox::warning(this, QStringLiteral("文件不存在"), QStringLiteral("文件不存在，请检查文件名"));
		return;
	}
	//
	vector<int> il = fp.GetItemList();
	modelclear();
	int crow = 0;
	for (int i = 0; i < il.size(); i++) {
		QList<QStandardItem*> temp_list;
		if (citemInfo.id2name.find(il[i]) == citemInfo.id2name.end()) {
			temp_list << new QStandardItem(QStringLiteral("未知家具"));
		}
		else {
			string cstr = citemInfo.id2name[il[i]];
			temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
		}
		temp_list << new QStandardItem(QString::number(il[i]));
		model->insertRow(crow++, temp_list);
	}

	//

	ItemList t_itemList;
	vector<ItemList::CategoryInstance> _itemList = t_itemList.ReadCateListJson(c2);
	//vector<int> il = fp.GetItemList();

	int err_a=-1, err_b=-1, err_c=-1, err_d=-1;

	int res = t_itemList.Compare(il, _itemList,err_a,err_b,err_c,err_d);
	if (res == 1) {
		this->setWindowTitle("1");
	}
	else {
		this->setWindowTitle("0");
	}
	cout << "qt " << err_a <<" "<<err_b << " " << err_c << " " << err_d << endl;
	if (err_a == 1) {
		//err b=tid  c=tcount d=json_count
		QString title = QStringLiteral("数量不匹配:id=")+QString::number(err_b);
		QString itemname;
		if (citemInfo.id2name.find(err_b) == citemInfo.id2name.end()) {
			itemname = QStringLiteral("未知家具");
		}
		else {
			string cstr = citemInfo.id2name[err_b];
			itemname = QString::fromLocal8Bit(cstr.c_str());
		}
		QString qstr = QStringLiteral("家具：") + QString::number(err_b) + QStringLiteral("(") + itemname + QStringLiteral(")")
			+ QStringLiteral("在当前家具列表中数量为") + QString::number(err_c) + QStringLiteral(",但在匹配文件中为") + QString::number(err_d);
		QMessageBox::warning(this, title, qstr);
	}
	else if (err_a == 2) {
		//err b=tid not found in JSON
		QString title = QStringLiteral("种类不匹配:id=") + QString::number(err_b);
		QString itemname;
		if (citemInfo.id2name.find(err_b) == citemInfo.id2name.end()) {
			itemname = QStringLiteral("未知家具");
		}
		else {
			string cstr = citemInfo.id2name[err_b];
			itemname = QString::fromLocal8Bit(cstr.c_str());
		}
		QString qstr = QStringLiteral("家具：") + QString::number(err_b) + QStringLiteral("(") + itemname + QStringLiteral(")")
			+ QStringLiteral("出现在当前家具列表中，但未出现在匹配文件中");
		QMessageBox::warning(this, title, qstr);
	}
	else if (err_a == 3) {
		//err b=cid not found in your house
		QString title = QStringLiteral("种类不匹配:id=") + QString::number(err_b);
		QString itemname;
		if (citemInfo.id2name.find(err_b) == citemInfo.id2name.end()) {
			itemname = QStringLiteral("未知家具");
		}
		else {
			string cstr = citemInfo.id2name[err_b];
			itemname = QString::fromLocal8Bit(cstr.c_str());
		}
		QString qstr = QStringLiteral("家具：") + QString::number(err_b) + QStringLiteral("(") + itemname + QStringLiteral(")")
			+ QStringLiteral("出现在匹配文件中，但未出现在当前家具列表中");
		QMessageBox::warning(this, title, qstr);
	}
	
}
void IMPORT::pb_2_on_clicked() {
	QString s1 = ui.le_import->text();
	QString ffp = ".\\list\\" + s1;
	QByteArray ba2;
	ba2.append(ffp);     
	const char* c2 = ba2.data();
	ItemList t_itemList;
	vector<ItemList::CategoryInstance> _itemList = t_itemList.ReadCateListJson(c2);
	vector<int> il = fp.GetItemList();
	f_xyzr = t_itemList.GetPosRotationOfVectorIntByCategoryList(il, _itemList);
	//
	modelclear();
	int crow = 0;
	for (int i = 0; i < il.size(); i++) {
		QList<QStandardItem*> temp_list;
		if (citemInfo.id2name.find(il[i]) == citemInfo.id2name.end()) {
			temp_list << new QStandardItem(QStringLiteral("未知家具"));
		}
		else {
			string cstr = citemInfo.id2name[il[i]];
			temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
		}
		temp_list << new QStandardItem(QString::number(il[i]));
		temp_list << new QStandardItem(QString::number(f_xyzr[0][i]));
		temp_list << new QStandardItem(QString::number(f_xyzr[1][i]));
		temp_list << new QStandardItem(QString::number(f_xyzr[2][i]));
		temp_list << new QStandardItem(QString::number(f_xyzr[3][i]));
		model->insertRow(crow++, temp_list);
	}


}
void IMPORT::pb_3_on_clicked() {
	for (int i = 5; i >= 0; i--) {
		this->setWindowTitle(QString::number(i));
		Sleep(1000);
	}
	vector<vector<float> >temp;
	vector<int> il = fp.GetItemList();
	for (int i = 0; i < il.size(); i++) {
		vector<float> ixyzr;
		ixyzr.push_back(f_xyzr[0][i]);
		ixyzr.push_back(f_xyzr[1][i]);
		ixyzr.push_back(f_xyzr[2][i]);
		ixyzr.push_back(f_xyzr[3][i]);
		temp.push_back(ixyzr);
	}
	
	//fp.SetAllItemPos(temp,s_t2,s_t0,s_tn);
	fp.SetAllItemPosRotation(temp, s_t2, s_t0, s_tn);
}

void IMPORT::pb_dllbf_on_clicked() {
	FFHook& ffHook = FFHook::get_instance();
	ffHook.setInit(1);
}


/*
#define BUFSIZE 512
LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\ffget");
const int BUFFER_MAX_LEN = 1024;
char buf[BUFFER_MAX_LEN];
DWORD dwLen;
HANDLE get, mSend, mutex;
LPCRITICAL_SECTION cs;
void beginGetThread(PVOID p) {
	printf("服务器Get\n");
	printf("等待连接......\n");
	HANDLE hPipe = CreateNamedPipe(
		lpszPipename,             // pipe name
		PIPE_ACCESS_DUPLEX,       // read/write access
		PIPE_TYPE_MESSAGE |       // message type pipe
		PIPE_READMODE_MESSAGE |   // message-read mode
		PIPE_WAIT,                // blocking mode
		PIPE_UNLIMITED_INSTANCES, // max. instances
		BUFSIZE,                  // output buffer size
		BUFSIZE,                  // input buffer size
		0,                        // client time-out
		NULL);                    // default security attribute
	printf("创建成功,%d,%d\n", hPipe, GetLastError());
	if (ConnectNamedPipe(hPipe, NULL) != NULL)//等待连接。
	{
		printf("连接成功，开始接收数据\n");
		while (true)
		{
			WaitForSingleObject(mutex, INFINITE);
			EnterCriticalSection(cs);
			//接收客户端发送的数据
			ReadFile(hPipe, buf, BUFFER_MAX_LEN, &dwLen, NULL);
			printf("接收到来自A的数据长度为%d字节\n", dwLen);
			printf("具体数据内容如下：");
			int bufSize;
			for (bufSize = 0; bufSize < (int)dwLen; bufSize++) {
				//putchar(buf[bufSize]);
				printf("%02x ", buf[bufSize]);
			}
			if ((int)dwLen == 2 && (buf[0] == 0x03 && buf[1] == 0x03)) { printf("injsuc\n"); return; }
			LeaveCriticalSection(cs);
			Sleep(500);
			ReleaseSemaphore(mutex, 1, NULL);
			putchar('\n');
		}
	}
	else
	{
		printf("连接失败,%d\n", GetLastError());
	}
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);//关闭管道
}
*/