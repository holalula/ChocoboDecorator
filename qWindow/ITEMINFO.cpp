#include "ITEMINFO.h"
#include "FFJson.h"
#include <io.h>
#include <QMessageBox>
#include <qDebug>

ITEMINFO::ITEMINFO(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pb_read, SIGNAL(clicked()), this, SLOT(pb_read_on_clicked()));
	connect(ui.pb_write, SIGNAL(clicked()), this, SLOT(pb_write_on_clicked()));
	connect(ui.pb_wline, SIGNAL(clicked()), this, SLOT(pb_wline_on_clicked()));
	connect(ui.pb_dline, SIGNAL(clicked()), this, SLOT(pb_dline_on_clicked()));

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
}
ITEMINFO::~ITEMINFO()
{
}
void ITEMINFO::modelclear() {
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



void ITEMINFO::pb_dline_on_clicked() {
	QItemSelectionModel* selections = ui.tableView->selectionModel();
	//QModelIndexList selected = selections->selectedIndexes();
	QModelIndex qmIndex = selections->currentIndex();
	int r = qmIndex.row();
	ui.tableView->model()->removeRow(r);

}

void ITEMINFO::pb_read_on_clicked() {
	modelclear();
	QString s1 = ui.le_impath->text();
	QString fp = ".\\list\\" + s1;

	QByteArray ba2;
	ba2.append(fp);
	const char* c2 = ba2.data();
	if ((_access(c2, 0)) == -1) {
		QMessageBox::warning(this, QStringLiteral("文件不存在"), QStringLiteral("文件不存在，请检查文件名"));
		return;
	}
	//read .json
	Document jitemlist;
	FILE* fp_r;
	fopen_s(&fp_r, c2, "r+");
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

			if (citemInfo.id2name.find(category) == citemInfo.id2name.end()&&citemInfo.id2name2.find(category)== citemInfo.id2name2.end()) {
				temp_list << new QStandardItem(QStringLiteral("未知家具"));
			}
			else if (citemInfo.id2name.find(category) != citemInfo.id2name.end()){
				string cstr = citemInfo.id2name[category];
				temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
			}
			else if (citemInfo.id2name2.find(category) != citemInfo.id2name2.end()) {
				string cstr = citemInfo.id2name2[category];
				temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
			}
			temp_list << new QStandardItem(QString::number(category, 10));
			temp_list << new QStandardItem(QString::number(j + 1));
			temp_list << new QStandardItem(QString::number(jitemlist["list"][i]["posX"][j].GetDouble()));
			temp_list << new QStandardItem(QString::number(jitemlist["list"][i]["posY"][j].GetDouble()));
			temp_list << new QStandardItem(QString::number(jitemlist["list"][i]["posZ"][j].GetDouble()));
			temp_list << new QStandardItem(QString::number(jitemlist["list"][i]["Rotation"][j].GetDouble()));
			model->insertRow(crow++, temp_list);
		}

	}

	return;
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
void ITEMINFO::pb_write_on_clicked() {
	//
	QString s1 = ui.le_expath->text();
	QString fp = ".\\list\\" + s1;
	QByteArray ba2;
	ba2.append(fp);
	const char* c2 = ba2.data();
	//c2 FilePath
	int rc = model->rowCount();
	ui.le_impath->setText(QString::number(rc));

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
	fopen_s(&fp_w, c2, "w+");
	char writeBuffer[512];
	FileWriteStream os(fp_w, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp_w);

	return;
}
void ITEMINFO::pb_wline_on_clicked() {
	int rcount = ui.tableView->model()->rowCount();
	qDebug() << rcount << endl;

	int cid = ui.le_id->text().toInt();
	//std::string cname = ui.le_name->text().toStdString();
	//qDebug() << citemInfo.name2id[cname] << endl;
	ui.le_name->setText(QString::fromLocal8Bit(citemInfo.id2name[cid].c_str()));

	QList<QStandardItem*> temp_list;
//
	if (citemInfo.id2name.find(cid) == citemInfo.id2name.end()) {
		temp_list << new QStandardItem(QStringLiteral("未知家具"));
	}
	else {
		string cstr = citemInfo.id2name[cid];
		temp_list << new QStandardItem(QString::fromLocal8Bit(cstr.c_str()));
	}
//
	//temp_list << new QStandardItem(ui.le_name->text());
	temp_list << new QStandardItem(ui.le_id->text());
	//where to insert
	QModelIndex mindex;
	int rc = model->rowCount();
	bool find = false;
	for (int i = 0; i < rc; i++) {
		mindex = model->index(i, 1);
		int i_cid = model->data(mindex).toInt();
		if (i < rc - 1) {
			mindex = model->index(i + 1, 1);
			int i1_cid = model->data(mindex).toInt();
			if (cid == i_cid && cid != i1_cid) {
				find = true;
				rcount = i + 1;
				mindex = model->index(i, 2);
				int order = model->data(mindex).toInt();
				temp_list << new QStandardItem(QString::number(order+1));
				break;
			}
		}
		else if(i==rc-1){
			mindex = model->index(i, 1);
			int temp_id = model->data(mindex).toInt();
			if (temp_id == cid) {
				find = true;
				mindex = model->index(i, 2);
				int order = model->data(mindex).toInt();
				temp_list << new QStandardItem(QString::number(order + 1));
				break;
			}
		}
	}
	if (!find) {
		temp_list << new QStandardItem(QString::number(1));
	}
	model->insertRow(rcount, temp_list);
}
