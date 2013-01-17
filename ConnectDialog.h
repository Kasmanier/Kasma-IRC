#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_ConnectDialog.h"

class ConnectDialog : public QDialog, public Ui_ConnectDialog
{
		Q_OBJECT
		
	public:
		ConnectDialog(QWidget *parent = 0);
		QString getServerName();
		qint32 getServerPort();
		QString getNickName();
		
	private:
		Ui::ConnectDialog *ui;
};

#endif // CONNECTDIALOG_H
