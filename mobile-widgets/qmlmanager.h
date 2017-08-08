// SPDX-License-Identifier: GPL-2.0
#ifndef QMLMANAGER_H
#define QMLMANAGER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QScreen>
#include <QElapsedTimer>
#include <QColor>

#include "core/btdiscovery.h"
#include "core/gpslocation.h"
#include "core/downloadfromdcthread.h"
#include "qt-models/divelistmodel.h"

class QMLManager : public QObject {
	Q_OBJECT
	Q_ENUMS(credentialStatus_t)
	Q_PROPERTY(QString cloudUserName READ cloudUserName WRITE setCloudUserName NOTIFY cloudUserNameChanged)
	Q_PROPERTY(QString cloudPassword READ cloudPassword WRITE setCloudPassword NOTIFY cloudPasswordChanged)
	Q_PROPERTY(QString cloudPin READ cloudPin WRITE setCloudPin NOTIFY cloudPinChanged)
	Q_PROPERTY(QString logText READ logText WRITE setLogText NOTIFY logTextChanged)
	Q_PROPERTY(bool locationServiceEnabled READ locationServiceEnabled WRITE setLocationServiceEnabled NOTIFY locationServiceEnabledChanged)
	Q_PROPERTY(bool locationServiceAvailable READ locationServiceAvailable WRITE setLocationServiceAvailable NOTIFY locationServiceAvailableChanged)
	Q_PROPERTY(int distanceThreshold READ distanceThreshold WRITE setDistanceThreshold NOTIFY distanceThresholdChanged)
	Q_PROPERTY(int timeThreshold READ timeThreshold WRITE setTimeThreshold NOTIFY timeThresholdChanged)
	Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
	Q_PROPERTY(bool loadFromCloud READ loadFromCloud WRITE setLoadFromCloud NOTIFY loadFromCloudChanged)
	Q_PROPERTY(QString startPageText READ startPageText WRITE setStartPageText NOTIFY startPageTextChanged)
	Q_PROPERTY(bool verboseEnabled READ verboseEnabled WRITE setVerboseEnabled NOTIFY verboseEnabledChanged)
	Q_PROPERTY(credentialStatus_t credentialStatus READ credentialStatus WRITE setCredentialStatus NOTIFY credentialStatusChanged)
	Q_PROPERTY(credentialStatus_t oldStatus READ oldStatus WRITE setOldStatus NOTIFY oldStatusChanged)
	Q_PROPERTY(QString notificationText READ notificationText WRITE setNotificationText NOTIFY notificationTextChanged)
	Q_PROPERTY(bool syncToCloud READ syncToCloud WRITE setSyncToCloud NOTIFY syncToCloudChanged)
	Q_PROPERTY(int updateSelectedDive READ updateSelectedDive WRITE setUpdateSelectedDive NOTIFY updateSelectedDiveChanged)
	Q_PROPERTY(int selectedDiveTimestamp READ selectedDiveTimestamp WRITE setSelectedDiveTimestamp NOTIFY selectedDiveTimestampChanged)
	Q_PROPERTY(QStringList suitInit READ suitInit CONSTANT)
	Q_PROPERTY(QStringList buddyInit READ buddyInit CONSTANT)
	Q_PROPERTY(QStringList divemasterInit READ divemasterInit CONSTANT)
	Q_PROPERTY(QStringList cylinderInit READ cylinderInit CONSTANT)
	Q_PROPERTY(bool showPin READ showPin WRITE setShowPin NOTIFY showPinChanged)
	Q_PROPERTY(QString progressMessage READ progressMessage WRITE setProgressMessage NOTIFY progressMessageChanged)
	Q_PROPERTY(bool libdcLog READ libdcLog WRITE setLibdcLog NOTIFY libdcLogChanged)
	Q_PROPERTY(bool developer READ developer WRITE setDeveloper NOTIFY developerChanged)

public:
	QMLManager();
	~QMLManager();

	enum credentialStatus_t {
		INCOMPLETE,
		UNKNOWN,
		INVALID,
		VALID_EMAIL,
		VALID,
		NOCLOUD
	};

	static QMLManager *instance();

	QString cloudUserName() const;
	void setCloudUserName(const QString &cloudUserName);

	QString cloudPassword() const;
	void setCloudPassword(const QString &cloudPassword);

	QString cloudPin() const;
	void setCloudPin(const QString &cloudPin);

	bool locationServiceEnabled() const;
	void setLocationServiceEnabled(bool locationServiceEnable);

	bool locationServiceAvailable() const;
	void setLocationServiceAvailable(bool locationServiceAvailable);

	bool verboseEnabled() const;
	void setVerboseEnabled(bool verboseMode);

	int distanceThreshold() const;
	void setDistanceThreshold(int distance);

	int timeThreshold() const;
	void setTimeThreshold(int time);

	QString theme() const;
	void setTheme(QString theme);

	bool loadFromCloud() const;
	void setLoadFromCloud(bool done);
	void syncLoadFromCloud();

	QString startPageText() const;
	void setStartPageText(const QString& text);

	credentialStatus_t credentialStatus() const;
	void setCredentialStatus(const credentialStatus_t value);

	credentialStatus_t oldStatus() const;
	void setOldStatus(const credentialStatus_t value);

	QString logText() const;
	void setLogText(const QString &logText);

	QString notificationText() const;
	void setNotificationText(QString text);

	bool syncToCloud() const;
	void setSyncToCloud(bool status);

	int updateSelectedDive() const;
	void setUpdateSelectedDive(int idx);

	int selectedDiveTimestamp() const;
	void setSelectedDiveTimestamp(int when);

	QString progressMessage() const;
	void setProgressMessage(QString text);

	bool libdcLog() const;
	void setLibdcLog(bool value);

	bool developer() const;
	void setDeveloper(bool value);

	typedef void (QMLManager::*execute_function_type)();
	DiveListSortModel *dlSortModel;

	QStringList suitInit() const;
	QStringList buddyInit() const;
	QStringList divemasterInit() const;
	QStringList cylinderInit() const;
	bool showPin() const;
	void setShowPin(bool enable);
	Q_INVOKABLE void setStatusbarColor(QColor color);
	Q_INVOKABLE bool btEnabled() const;

#if defined(Q_OS_ANDROID)
	void writeToAppLogFile(QString logText);
#endif

public slots:
	void applicationStateChanged(Qt::ApplicationState state);
	void savePreferences();
	void saveCloudCredentials();
	void checkCredentialsAndExecute(execute_function_type execute);
	void tryRetrieveDataFromBackend();
	void handleError(QNetworkReply::NetworkError nError);
	void handleSslErrors(const QList<QSslError> &errors);
	void retrieveUserid();
	void loadDivesWithValidCredentials();
	void provideAuth(QNetworkReply *reply, QAuthenticator *auth);
	void commitChanges(QString diveId, QString date, QString location, QString gps,
			   QString duration, QString depth, QString airtemp,
			   QString watertemp, QString suit, QString buddy,
			   QString diveMaster, QString weight, QString notes, QString startpressure,
			   QString endpressure, QString gasmix, QString cylinder, int rating, int visibility);
	void changesNeedSaving();
	void saveChangesLocal();
	void saveChangesCloud(bool forceRemoteSync);
	void deleteDive(int id);
	bool undoDelete(int id);
	QString addDive();
	void addDiveAborted(int id);
	void applyGpsData();
	void sendGpsData();
	void downloadGpsData();
	void populateGpsData();
	void cancelDownloadDC();
	void clearGpsData();
	void finishSetup();
	void openLocalThenRemote(QString url);
	void mergeLocalRepo();
	QString getNumber(const QString& diveId);
	QString getDate(const QString& diveId);
	QString getCurrentPosition();
	QString getVersion() const;
	void deleteGpsFix(quint64 when);
	void revertToNoCloudIfNeeded();
	void consumeFinishedLoad(timestamp_t currentDiveTimestamp);
	void refreshDiveList();
	void screenChanged(QScreen *screen);
	qreal lastDevicePixelRatio();
	void setDevicePixelRatio(qreal dpr, QScreen *screen);
	void appendTextToLog(const QString &newText);
	void quit();
	void hasLocationSourceChanged();

private:
	QString m_cloudUserName;
	QString m_cloudPassword;
	QString m_cloudPin;
	QString m_ssrfGpsWebUserid;
	QString m_startPageText;
	QString m_logText;
	bool m_locationServiceEnabled;
	bool m_locationServiceAvailable;
	bool m_verboseEnabled;
	int m_distanceThreshold;
	int m_timeThreshold;
	GpsLocation *locationProvider;
	bool m_loadFromCloud;
	static QMLManager *m_instance;
	QNetworkReply *reply;
	QNetworkRequest request;
	struct dive *deletedDive;
	struct dive_trip *deletedTrip;
	QString m_notificationText;
	bool m_syncToCloud;
	int m_updateSelectedDive;
	int m_selectedDiveTimestamp;
	credentialStatus_t m_credentialStatus;
	credentialStatus_t m_oldStatus;
	qreal m_lastDevicePixelRatio;
	QElapsedTimer timer;
	bool alreadySaving;
	bool checkDate(DiveObjectHelper *myDive, struct dive * d, QString date);
	bool checkLocation(DiveObjectHelper *myDive, struct dive *d, QString location, QString gps);
	bool checkDuration(DiveObjectHelper *myDive, struct dive *d, QString duration);
	bool checkDepth(DiveObjectHelper *myDive, struct dive *d, QString depth);
	bool currentGitLocalOnly;
	bool m_showPin;
	DCDeviceData *m_device_data;
	QString m_progressMessage;
	bool m_libdcLog;
	bool m_developer;
	bool m_btEnabled;

#if defined(Q_OS_ANDROID)
	QString appLogFileName;
	QFile appLogFile;
	bool appLogFileOpen;
#endif

signals:
	void cloudUserNameChanged();
	void cloudPasswordChanged();
	void cloudPinChanged();
	void locationServiceEnabledChanged();
	void locationServiceAvailableChanged();
	void verboseEnabledChanged();
	void logTextChanged();
	void timeThresholdChanged();
	void themeChanged();
	void distanceThresholdChanged();
	void loadFromCloudChanged();
	void startPageTextChanged();
	void credentialStatusChanged();
	void oldStatusChanged();
	void notificationTextChanged();
	void syncToCloudChanged();
	void updateSelectedDiveChanged();
	void selectedDiveTimestampChanged();
	void showPinChanged();
	void sendScreenChanged(QScreen *screen);
	void progressMessageChanged();
	void libdcLogChanged();
	void developerChanged();
};

#endif
