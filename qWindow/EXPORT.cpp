#include "EXPORT.h"

#include "include/rapidjson/document.h"
#include "include/rapidjson/stringbuffer.h"
#include "include/rapidjson/filereadstream.h"
#include "include/rapidjson/filewritestream.h"
#include "include/rapidjson/writer.h"

#include <vector>
#include <fstream>
#include <QString>

#include <QDebug>

using namespace std;
using namespace rapidjson;

EXPORT::EXPORT(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	connect(ui.pb_export, SIGNAL(clicked()), this, SLOT(pb_export_on_clicked()));
	connect(ui.pb_import, SIGNAL(clicked()), this, SLOT(pb_import_on_clicked()));
	connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(cb_checkBox_on_stateChanged(int)));
	saveCount = 0;


	ui.tableView->setMouseTracking(true);
	model = new QStandardItemModel(this);
	model->setColumnCount(7);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("序列"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("X"));
	model->setHeaderData(4, Qt::Horizontal, QStringLiteral("Y"));
	model->setHeaderData(5, Qt::Horizontal, QStringLiteral("Z"));
	model->setHeaderData(6, Qt::Horizontal, QStringLiteral("R"));
	
	ui.tableView->setModel(model);
	citemInfo.update();

	FFJson j_d;
	j_d.ReadJsonFile("delaytime.json");
	delay_t2 = j_d.document["e_t2"].GetInt();
	delay_t0 = j_d.document["e_t0"].GetInt();
	delay_tn = j_d.document["e_tn"].GetInt();

}

EXPORT::~EXPORT()
{
}
void EXPORT::cb_checkBox_on_stateChanged(int a) {
	if (a == Qt::Checked) {
		//
		for (int i = 5; i >= 0;i--) {
			this->setWindowTitle(QString::number(i));
			Sleep(1000);
		}
		fp.InjectBlue();
		vector<int> itemList = fp.GetItemList();

		qDebug() << itemList.size() << endl;

		vector<vector<float> > pos = fp.GetAllItemPosRotation(1,delay_t2,delay_t0,delay_tn);
		int itemCount = itemList.size();

		//Set Table
		modelclear();
		int samecategoryIndex = 1;
		int crow = 0;
		for (int i = 0; i < itemCount; i++) {
			QList<QStandardItem*> temp_list;
			if (citemInfo.id2name.find(itemList[i]) == citemInfo.id2name.end()) {
				temp_list << new QStandardItem(QStringLiteral("未知家具"));
			}
			else {
				string cstr = citemInfo.id2name[itemList[i]];
				temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
			}
			temp_list << new QStandardItem(QString::number(itemList[i]));
			temp_list << new QStandardItem(QString::number(samecategoryIndex));
			temp_list << new QStandardItem(QString::number(pos[0][i]));
			temp_list << new QStandardItem(QString::number(pos[1][i]));
			temp_list << new QStandardItem(QString::number(pos[2][i]));
			temp_list << new QStandardItem(QString::number(pos[3][i]));
			model->insertRow(crow++, temp_list);
			samecategoryIndex++;
			if (itemList[i] != itemList[i + 1]) {
				samecategoryIndex = 1;
			}
		}
		

	}
	else {
		
	}
	fp.reInjectBlue();
}

void EXPORT::SetFp(FFProcess* t) {
	fp = *t;
}

void EXPORT::modelclear() {
	model->clear();
	model->setColumnCount(7);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("序列"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("X"));
	model->setHeaderData(4, Qt::Horizontal, QStringLiteral("Y"));
	model->setHeaderData(5, Qt::Horizontal, QStringLiteral("Z"));
	model->setHeaderData(6, Qt::Horizontal, QStringLiteral("R"));
}

//window size event
void EXPORT::resizeEvent(QResizeEvent* event)
{
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
//tableview slot
void EXPORT::on_tableView_entered(const QModelIndex& index)
{
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}

//c1
int EXPORT::loadItemListJson(const char* FilePath) {
	//read .json
	Document jitemlist;
	FILE* fp_r;
	fopen_s(&fp_r, FilePath, "r+");
	char readBuffer[512];
	FileReadStream is(fp_r, readBuffer, sizeof(readBuffer));
	jitemlist.ParseStream(is);
	fclose(fp_r);

	int listSize = jitemlist["size"].GetInt();
	int crow = 0;
	for (int i = 0; i < listSize; i++) {
		int category = jitemlist["list"][i]["categoryId"].GetInt();
		int count = jitemlist["list"][i]["count"].GetInt();
		for (int j = 0; j < count; j++) {
			QList<QStandardItem*> temp_list;
			
			if (citemInfo.id2name.find(category)==citemInfo.id2name.end()) {
				temp_list << new QStandardItem(QStringLiteral("未知家具"));
			}
			else {
				string cstr = citemInfo.id2name[category];
				temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
			}
			temp_list << new QStandardItem(QString::number(category, 10));
			temp_list << new QStandardItem(QString::number(j+1));
			temp_list << new QStandardItem(QString::number(jitemlist["list"][i]["posX"][j].GetDouble()));
			temp_list << new QStandardItem(QString::number(jitemlist["list"][i]["posY"][j].GetDouble()));
			temp_list << new QStandardItem(QString::number(jitemlist["list"][i]["posZ"][j].GetDouble()));
			temp_list << new QStandardItem(QString::number(jitemlist["list"][i]["Rotation"][j].GetDouble()));
			model->insertRow(crow++, temp_list);
		}
		
	}

	return 0;
}

void EXPORT::pb_export_on_clicked() {
	QString s1 = ui.le_export->text();
	QString fp = ".\\list\\" + s1;
	if (saveCount != 0) {
		fp= ".\\list\\" +QString::number(saveCount)+ s1;
	}
	QByteArray ba2;
	ba2.append(fp);     
	const char* c2 = ba2.data();
	exportItemListJson(c2);
	saveCount++;
}
void EXPORT::pb_import_on_clicked() {
	model->clear();
	model->setColumnCount(7);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
	model->setHeaderData(2, Qt::Horizontal, QStringLiteral("序列"));
	model->setHeaderData(3, Qt::Horizontal, QStringLiteral("X"));
	model->setHeaderData(4, Qt::Horizontal, QStringLiteral("Y"));
	model->setHeaderData(5, Qt::Horizontal, QStringLiteral("Z"));
	model->setHeaderData(6, Qt::Horizontal, QStringLiteral("R"));

	QString s1 = ui.le_import->text();
	QString fp = ".\\list\\" + s1;

	QByteArray ba2;
	ba2.append(fp);     
	const char* c2 = ba2.data();
	loadItemListJson(c2);

}

class HelperCategoryInstance {
public:
	int categoryId;
	int count;
	vector<float> posX;
	vector<float> posY;
	vector<float> posZ;
	vector<float> rotation;
};

int EXPORT::exportItemListJson(const char* FilePath) {
	int rc = model->rowCount();
	ui.le_import->setText(QString::number(rc));

	vector<HelperCategoryInstance> categoryList;
	QModelIndex index;
	HelperCategoryInstance cat1;
	index = model->index(0, 1);
	cat1.categoryId = model->data(index).toInt();
	cat1.count = 0;
	categoryList.push_back(cat1);
	int cListindex = 0;
	for (int i = 0; i < rc; i++) {
		
		index = model->index(i, 1);
		int i_cid = model->data(index).toInt();

		index = model->index(i, 3);
		float x = model->data(index).toFloat();
		index = model->index(i, 4);
		float y = model->data(index).toFloat();
		index = model->index(i, 5);
		float z = model->data(index).toFloat();
		index = model->index(i, 6);
		float r = model->data(index).toFloat();
		categoryList[cListindex].count++;
		categoryList[cListindex].posX.push_back(x);
		categoryList[cListindex].posY.push_back(y);
		categoryList[cListindex].posZ.push_back(z);
		categoryList[cListindex].rotation.push_back(r);

		if (i < rc - 1) {
			index = model->index(i + 1, 1);
			int line_i1_cid = model->data(index).toInt();
			if (line_i1_cid != i_cid) {
				HelperCategoryInstance cat2;
				cat2.categoryId = line_i1_cid;
				cat2.count = 0;
				categoryList.push_back(cat2);
				cListindex++;
			}
		}

	}
	//Clist
	
	Document d;
	d.Parse("{}");
	d.AddMember("size", categoryList.size(), d.GetAllocator());
	Value d3(kArrayType);
	for (int j = 0; j < categoryList.size(); j++) {
		qDebug() << j << endl;
		HelperCategoryInstance temp = categoryList[j];
		char catId[4];
		_itoa(temp.categoryId, catId, 16);
		Value d2(kObjectType);
		d2.AddMember("categoryId", temp.categoryId, d.GetAllocator());
		d2.AddMember("count", temp.count, d.GetAllocator());
		
		// ->d2X
		Value arrX(kArrayType);
		for (int i = 0; i < temp.posX.size(); i++) {
			arrX.PushBack(temp.posX[i], d.GetAllocator());
		}
		d2.AddMember("posX", arrX, d.GetAllocator());
		// ->d2Y
		Value arrY(kArrayType);
		for (int i = 0; i < temp.posY.size(); i++) {
			arrY.PushBack(temp.posY[i], d.GetAllocator());
		}
		d2.AddMember("posY", arrY, d.GetAllocator());
		// ->d2Z
		Value arrZ(kArrayType);
		for (int i = 0; i < temp.posZ.size(); i++) {
			arrZ.PushBack(temp.posZ[i], d.GetAllocator());
		}
		d2.AddMember("posZ", arrZ, d.GetAllocator());
		// ->d2R
		Value arrR(kArrayType);
		for (int i = 0; i < temp.rotation.size(); i++) {
			arrR.PushBack(temp.rotation[i], d.GetAllocator());
		}
		d2.AddMember("Rotation", arrR, d.GetAllocator());
		//string strId(catId);
		//const char* ff = catId;
		d3.PushBack(d2, d.GetAllocator());
	}

	d.AddMember("list", d3, d.GetAllocator());
	
	//JSON d
	FILE* fp_w;
	fopen_s(&fp_w, FilePath, "w+");
	char writeBuffer[512];
	FileWriteStream os(fp_w, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp_w);

	return 0;
}