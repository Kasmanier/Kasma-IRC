#include "ConnectDialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
}

QString ConnectDialog::getServerName()
{
	QString data = lineEditServerName->text();
	return data;
}

qint32 ConnectDialog::getServerPort()
{
	QString data = lineEditServerPort->text();
	return data.toInt();
}

QString ConnectDialog::getNickName()
{
	QString data = lineEditNickName->text();
	return data;
}

