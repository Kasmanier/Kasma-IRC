#ifndef PARSERINPUT_H
#define PARSERINPUT_H

#include <QString>
#include <QStringList>
#include <QRegExp>

class ParserInput
{
	public:
		ParserInput();
		void setNames(QString const &server, QString const &nick);
		QStringList splitLine(QString const &line);

		bool isQuit(QString const &params);
		bool isChannelMessage(QStringList const &params);
		bool isPrivateMessage(QStringList const &params);

		QString getChannelMessageChannelName(QStringList const &params);
		QString getPrivateMessageChannelName(QStringList const &params);

	private:
		QString serverName;
		QString nickName;
};

#endif // PARSERINPUT_H
