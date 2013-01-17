#ifndef PARSEROUTPUT_H
#define PARSEROUTPUT_H

#include <QString>
#include <QStringList>
#include <QRegExp>

#include "Globals.h"

class ParserOutput
{
	public:
		ParserOutput();
		void setNames(QString const &server, QString const &nick);
		QStringList splitLine(QString const &line);

		bool isPing(QStringList const &params);
		bool isJoin(QStringList const &params);
		bool isOtherJoin(QStringList const &params);
		bool isPart(QStringList const &params);
		bool isOtherPart(QStringList const &params);
		bool isChannelMessage(QStringList const &params);
		bool isPrivateMessage(QStringList const &params);
		bool isNames(QStringList const &params);
		bool isChannelCode(QStringList const &params);

		QString getJoinChannelName(QStringList const &params);
		QString getPartChannelName(QStringList const &params);
		QString getChannelMessageChannelName(QStringList const &params);
		QString getPrivateMessageChannelName(QStringList const &params);
		QStringList getNamesChannelName(QStringList const &params);
		QString getChannelCodeChannelName(QStringList const &params);

		QStringList splitNickNameList(QStringList const &params);

	private:
		QString serverName;
		QString nickName;
};

#endif // PARSEROUTPUT_H
