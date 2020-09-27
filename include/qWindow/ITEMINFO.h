#pragma once

#include <QWidget>
#include "ui_ITEMINFO.h"
#include <QStandardItemModel>
#include <cstring>
#include <map>
#include "CItemInfo.h"
using namespace std;
class ITEMINFO : public QWidget
{
	Q_OBJECT

public:
	ITEMINFO(QWidget *parent = Q_NULLPTR);
	~ITEMINFO();
	QStandardItemModel* model;
	void modelclear();
	vector<string> name;
	vector<int> name_f;
	map<int, string> id2name;
	CItemInfo citemInfo;
	
public slots:
	void pb_read_on_clicked();
	void pb_write_on_clicked();
	void pb_wline_on_clicked();
	void pb_dline_on_clicked();
private:
	Ui::ITEMINFO ui;
};
