// SPDX-License-Identifier: GPL-2.0
#ifndef DIVEPICTUREMODEL_H
#define DIVEPICTUREMODEL_H

#include <QAbstractTableModel>
#include <QImage>
#include <QFuture>

struct PictureEntry {
	struct picture *picture;
	QString filename;
	QImage image;
	int offsetSeconds;
};

// function that will scale the pixmap, used inside the QtConcurrent thread.
void scaleImages(PictureEntry &entry);

class DivePictureModel : public QAbstractTableModel {
	Q_OBJECT
public:
	static DivePictureModel *instance();
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual void updateDivePictures();
	void updateDivePicturesWhenDone(QList<QFuture<void>>);
	void removePicture(const QString& fileUrl, bool last);

protected:
	DivePictureModel();
	QList<PictureEntry> pictures;
};

#endif
