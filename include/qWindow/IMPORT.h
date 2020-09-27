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
	void cb_checkBox_2_on_stateChanged(int);
	void pb_1_1_on_clicked();
	void pb_1_on_clicked();
	void pb_2_on_clicked();
	void pb_3_on_clicked();
	void pb_inject_on_clicked();
	void pb_einject_on_clicked();
	void pb_dllbf_on_clicked();
	void cb_iblue_on_stateChanged(int);

private:
	Ui::IMPORT ui;
	void modelclear();
	vector<vector<float> >f_xyzr;
	int s_t0;
	int s_t2;
	int s_tn;
	bool nativeEvent(const QByteArray& eventType, void* message, long* result);
	bool b_cmp = false;
};
