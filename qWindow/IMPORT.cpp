#include "IMPORT.h"

#include "ItemList.h"
#include <qDebug>
IMPORT::IMPORT(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.tableView->setMouseTracking(true);
	model = new QStandardItemModel(this);

	model->setColumnCount(5);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("X"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("Y"));
	model->setHeaderData(4, Qt::Horizontal, QStringLiteral("Z"));

	connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(cb_checkBox_on_stateChanged(int)));
	connect(ui.pb_1, SIGNAL(clicked()), this, SLOT(pb_1_on_clicked()));
	connect(ui.pb_2, SIGNAL(clicked()), this, SLOT(pb_2_on_clicked()));
	connect(ui.pb_3, SIGNAL(clicked()), this, SLOT(pb_3_on_clicked()));


	ui.tableView->setModel(model);

	FFJson j_r;
	j_r.ReadJsonFile("delaytime.json");
	s_t2 = j_r.document["s_t2"].GetInt();
	s_t0 = j_r.document["s_t0"].GetInt();
	s_tn = j_r.document["s_tn"].GetInt();

	this->citemInfo.update();


}

void IMPORT::modelclear() {
	model->clear();
	model->setColumnCount(5);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("X"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("Y"));
	model->setHeaderData(4, Qt::Horizontal, QStringLiteral("Z"));
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

IMPORT::~IMPORT()
{
}

void IMPORT::pb_1_on_clicked() {
	QString s1 = ui.le_import->text();
	QString ffp = ".\\list\\" + s1;
	QByteArray ba2;
	ba2.append(ffp);     
	const char* c2 = ba2.data();
	ItemList t_itemList;
	vector<ItemList::CategoryInstance> _itemList = t_itemList.ReadCateListJson(c2);
	vector<int> il = fp.GetItemList();
	int res = t_itemList.Compare(il, _itemList);
	if (res == 1) {
		this->setWindowTitle("1");
	}
	else {
		this->setWindowTitle("0");
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
	f_xyz = t_itemList.GetPosOfVectorIntByCategoryList(il, _itemList);
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
		temp_list << new QStandardItem(QString::number(f_xyz[0][i]));
		temp_list << new QStandardItem(QString::number(f_xyz[1][i]));
		temp_list << new QStandardItem(QString::number(f_xyz[2][i]));
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
		vector<float> ixyz;
		ixyz.push_back(f_xyz[0][i]);
		ixyz.push_back(f_xyz[1][i]);
		ixyz.push_back(f_xyz[2][i]);
		temp.push_back(ixyz);
	}
	
	fp.SetAllItemPos(temp,s_t2,s_t0,s_tn);

}

