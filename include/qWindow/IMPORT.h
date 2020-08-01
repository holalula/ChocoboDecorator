#pragma once

#include <QWidget>
#include "ui_IMPORT.h"
#include "QStandardItemModel"
#include "FFProcess.h"
#include "CItemInfo.h"
class IMPORT : public QWidget
{
	Q_OBJECT

public:
	QStandardItemModel* model;
	IMPORT(QWidget *parent = Q_NULLPTR);
	~IMPORT();
	void resizeEvent(QResizeEvent* event);
	void on_tableView_entered(const QModelIndex& index);
	void SetFp(FFProcess*);
	FFProcess fp;
	CItemInfo citemInfo;
public slots:
	void cb_checkBox_on_stateChanged(int);
	void pb_1_on_clicked();
	void pb_2_on_clicked();
	void pb_3_on_clicked();

private:
	Ui::IMPORT ui;
	void modelclear();
	vector<vector<float> >f_xyz;
	int s_t0;
	int s_t2;
	int s_tn;
};
