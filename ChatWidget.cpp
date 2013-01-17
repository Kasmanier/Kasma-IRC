#include "ChatWidget.h"

ChatWidget::ChatWidget(const QString &name, const qint32 type, QWidget *parent) : QTableWidget(parent)
{
	chatName = name;
	chatType = type;

	QHeaderView *headerViewHorizontal = new QHeaderView(Qt::Horizontal);
	headerViewHorizontal = horizontalHeader();
	headerViewHorizontal->setStretchLastSection(true);

	QHeaderView *headerViewVertical = new QHeaderView(Qt::Vertical);
	headerViewVertical = verticalHeader();
	headerViewVertical->setResizeMode(QHeaderView::ResizeToContents);

	horizontalHeader()->hide();
	verticalHeader()->hide();
	verticalHeader()->setContentsMargins(0, 0, 0, 0);
	verticalHeader()->setStyleSheet("QHeaderView:section { padding: 0px }");
	verticalHeader()->setStyleSheet("QHeaderView:section { margin: 0px }");

	setColumnCount(2);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setWordWrap(true);
}

void ChatWidget::addText(const QString &text)
{
	QTime time = QTime::currentTime();
	QString output = time.toString();

	int rows = rowCount();
	insertRow(rows);
	setItem(rows, 0, new QTableWidgetItem(output));
	setItem(rows, 1, new QTableWidgetItem(text));
	resizeColumnToContents(0);
}
