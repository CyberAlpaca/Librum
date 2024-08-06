#ifndef APPLICATION_INIT_H
#define APPLICATION_INIT_H

#include <QApplication>
#include <QQuickStyle>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

QPair<QPointer<QApplication>, QPointer<QQmlApplicationEngine>> initializeApplication(int argc, char *argv[]);
void setupGlobalSettings();
void loadFont(const QString& path);
void setupFonts();

#endif // APPLICATION_INIT_H
