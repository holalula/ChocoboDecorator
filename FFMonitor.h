#pragma once

#include <QWidget>
#include "ui_FFMonitor.h"
#include "FFProcess.h"
#include "CItemInfo.h"
#include <QStandardItemModel>
#include "FFHook.h"
class FFMonitor : public QWidget
{
	Q_OBJECT

public:
	FFMonitor(QWidget *parent = Q_NULLPTR);
	~FFMonitor();
	QStandardItemModel* model;
	void SetFp(FFProcess* t);
	CItemInfo citemInfo;
	FFHook& _ffhook= FFHook::get_instance();;
public slots:
	void pb_memo_on_clicked();
	void pb_net_on_clicked();

	void pb_getpath_on_clicked();
	void pb_injectff_on_clicked();
	void pb_loaddll_on_clicked();
	void pb_check_on_clicked();

	void pb_getinit_on_clicked();
	void pb_setinit_on_clicked();

	void pb_getmode_on_clicked();
	void pb_setmode_on_clicked();


	void pb_getx_on_clicked();
	void pb_setx_on_clicked();
	void pb_gety_on_clicked();
	void pb_sety_on_clicked();
	void pb_getz_on_clicked();
	void pb_setz_on_clicked();
	void pb_getr_on_clicked();
	void pb_setr_on_clicked();

private:
	Ui::FFMonitor ui;
	FFProcess fp;
	void modelclear();
};
