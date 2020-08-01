#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ChocoboFFXIV.h"
#include "windows.h"
#include "FFProcess.h"




class ChocoboFFXIV : public QMainWindow
{
    Q_OBJECT

public:
    ChocoboFFXIV(QWidget *parent = Q_NULLPTR);
    ~ChocoboFFXIV();
    FFProcess fp;
    QTimer* timer;
    QTimer* timer2;

public slots:
    void cb_placeanywhere_on_stateChanged(int a);
    void cb_activepos_on_stateChanged(int a);
    void le_px_on_editingFinished();
    void le_py_on_editingFinished();
    void le_pz_on_editingFinished();
	//void le_px_on_textEdited();
	//void le_py_on_textEdited();
	//void le_pz_on_textEdited();//textEdited
    void pb_export_on_clicked();
    void pb_import_on_clicked();
    void pb_copy_on_clicked();
    void updatePos();
    void checkwhere();

private:
    Ui::ChocoboFFXIVClass ui;
    void labelDebug(QString str);
    bool nativeEvent(const QByteArray& eventType, void* message, long* result);
    BOOL IsRunasAdmin();

};
