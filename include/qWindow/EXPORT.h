#pragma once

#include <QWidget>
#include "ui_EXPORT.h"
#include "QStandardItemModel"
#include "FFProcess.h"
#include "CItemInfo.h"
class EXPORT : public QWidget
{
	Q_OBJECT

public:
	FFProcess fp;
	QStandardItemModel* model;

	int saveCount;
	EXPORT(QWidget *parent = Q_NULLPTR);
	~EXPORT();
	void resizeEvent(QResizeEvent* event);
	void on_tableView_entered(const QModelIndex& index);
	int loadItemListJson(const char* FilePath);
	int exportItemListJson(const char* FilePath);
	void SetFp(FFProcess*);
	CItemInfo citemInfo;
public slots:
	void cb_checkBox_on_stateChanged(int);
	void pb_export_on_clicked();
	void pb_import_on_clicked();
private:
	Ui::EXPORT ui;
	void modelclear();
	int delay_t2;
	int delay_t0;
	int delay_tn;
};
