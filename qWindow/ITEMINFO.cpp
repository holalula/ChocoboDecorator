#include "ITEMINFO.h"
#include "FFJson.h"

#include <qDebug>
ITEMINFO::ITEMINFO(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pb_read, SIGNAL(clicked()), this, SLOT(pb_read_on_clicked()));
	connect(ui.pb_write, SIGNAL(clicked()), this, SLOT(pb_write_on_clicked()));
	connect(ui.pb_wline, SIGNAL(clicked()), this, SLOT(pb_wline_on_clicked()));

	ui.tableView->setMouseTracking(true);
	model = new QStandardItemModel(this);
	model->setColumnCount(2);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
	ui.tableView->setModel(model);

}

void ITEMINFO::modelclear() {
	model->clear();
	model->setColumnCount(2);
	model->setHeaderData(0, Qt::Horizontal, QStringLiteral("家具"));
	model->setHeaderData(1, Qt::Horizontal, QStringLiteral("类码"));
}

ITEMINFO::~ITEMINFO()
{
}

void ITEMINFO::pb_read_on_clicked() {
	FFJson j_itemlist;

	modelclear();
	name.clear();
	name_f.clear();
	int crow = 0;

	j_itemlist.ReadJsonFile("ItemList.json");
	for (Value::ConstMemberIterator itr = j_itemlist.document.MemberBegin(); itr != j_itemlist.document.MemberEnd(); itr++) {
		Value jkey;
		Value jvalue;
		Document::AllocatorType allocator;
		jkey.CopyFrom(itr->name, allocator);
		jvalue.CopyFrom(itr->value, allocator);
		if (jkey.IsString()) {
			if (jvalue.IsInt()) {
				string strKey = jkey.GetString();
				
				int intValue = jvalue.GetInt();
				name.push_back(strKey);
				name_f.push_back(intValue);
				QList<QStandardItem*> temp_list;
				temp_list << new QStandardItem(QString::fromLocal8Bit(strKey.c_str()));
				temp_list << new QStandardItem(QString::number(intValue));
				model->insertRow(crow++, temp_list);
			}
		}
	}
}
void ITEMINFO::pb_write_on_clicked() {
	//
}
void ITEMINFO::pb_wline_on_clicked() {
	int rcount = ui.tableView->model()->rowCount();
	qDebug() << rcount << endl;


	QList<QStandardItem*> temp_list;
	temp_list << new QStandardItem(ui.le_name->text());
	temp_list << new QStandardItem(ui.le_id->text());
	model->insertRow(rcount, temp_list);
}
