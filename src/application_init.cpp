#include "application_init.h"

#include <qdiriterator.h>
#include <qfontdatabase.h>
#include <qqml.h>
#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QGuiApplication>
#include <QIcon>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSettings>
#include <QStandardPaths>
#include <QString>
#include <QTextStream>
#include <QTranslator>
#include <iostream>
#include <memory>
#include <tools_service.hpp>
#include "app_info_controller.hpp"
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "book_service.hpp"
#include "dependency_injection.hpp"
#include "external_book_controller.hpp"
#include "folder_dto.hpp"
#include "free_books_model.hpp"
#include "i_free_books_service.hpp"
#include "i_library_service.hpp"
#include "i_user_service.hpp"
#include "key_sequence_recorder.hpp"
#include "library_proxy_model.hpp"
#include "message_handler.hpp"
#include "page_view.hpp"
#include "setting_groups.hpp"
#include "setting_keys.hpp"
#include "shortcuts_proxy_model.hpp"
#include "tag_dto.hpp"
#include "tools_controller.hpp"
#include "user_controller.hpp"
#include "word_definition_dto.hpp"
#include "global_controllers.h"


using namespace adapters::controllers;
using namespace application::services;


void registerTypes();
void setupGlobalSettings();
void setupFonts();

QPair<QPointer<QApplication>, QPointer<QQmlApplicationEngine>> initializeApplication(int argc, char *argv[]) {
    // clang-format off
    // App
    // QApplication app(argc, argv);
    QPointer<QApplication> app = new QApplication(argc, argv);
    QApplication::setOrganizationName("Librum-Reader");
    QApplication::setOrganizationDomain("librumreader.com");
    QApplication::setApplicationName("Librum");
    QQuickStyle::setStyle(QStringLiteral("Default"));

    QIcon icon(":/src/logo.ico");
    app->setWindowIcon(icon);

    qInstallMessageHandler(logging::messageHandler);
    setupGlobalSettings();
    setupFonts();

            // Register types
    qmlRegisterSingletonType(QUrl("qrc:/StyleSheet.qml"), "Librum.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl("qrc:/IconSheet.qml"), "Librum.icons", 1, 0, "Icons");
    qmlRegisterSingletonType(QUrl("qrc:/FontSheet.qml"), "Librum.fonts", 1, 0, "Fonts");
    qmlRegisterSingletonType(QUrl("qrc:/Globals.qml"), "Librum.globals", 1, 0, "Globals");
    qmlRegisterSingletonType(QUrl("qrc:/GlobalSettings.qml"), "Librum.globalSettings", 1, 0, "GlobalSettings");
    qmlRegisterSingletonType(QUrl("qrc:/TranslationsModel.qml"), "Librum.models", 1, 0, "TranslationsModel");
    qmlRegisterSingletonType(QUrl("qrc:/modules/CustomComponents/MLanguageModel.qml"), "Librum.models", 1, 0, "MLanguageModel");
    qmlRegisterType<adapters::data_models::LibraryProxyModel>("Librum.models", 1, 0, "LibraryProxyModel");
    qmlRegisterType<adapters::data_models::BookTitleProxyModel>("Librum.models", 1, 0, "BookTitleModel");
    qmlRegisterType<adapters::data_models::FreeBooksModel>("Librum.models", 1, 0, "FreeBooksModel");
    qmlRegisterType<adapters::data_models::ShortcutsProxyModel>("Librum.models", 1, 0, "ShortcutsProxyModel");
    qmlRegisterType<cpp_elements::KeySequenceRecorder>("Librum.elements", 1, 0, "KeySequenceRecorder");
    qmlRegisterType<cpp_elements::PageView>("Librum.elements", 1, 0, "PageView");
    qRegisterMetaType<adapters::dtos::BookDto>();
    qRegisterMetaType<adapters::dtos::TagDto>();
    qRegisterMetaType<adapters::dtos::FolderDto>();
    qRegisterMetaType<adapters::dtos::DictionaryEntryDto>();
    qRegisterMetaType<adapters::dtos::WordTypeDto>();
    qRegisterMetaType<adapters::dtos::WordDefinitionDto>();


            // Authentication Stack
    auto* authenticationService =
        config::diConfig().create<application::IAuthenticationService*>();
    GlobalControllers::authenticationController =
        std::make_unique<AuthenticationController>(authenticationService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AuthController",
                                 GlobalControllers::authenticationController.get());

            // App Info Stack
    auto* appInfoService =
        config::diConfig().create<application::IAppInfoService*>();
    GlobalControllers::appInfoController =
        std::make_unique<AppInfoController>(appInfoService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AppInfoController",
                                 GlobalControllers::appInfoController.get());

            // Ai explanation
    auto* aiExplanationService =
        config::diConfig().create<application::IAiExplanationService*>();
    GlobalControllers::aiExplanationController =
        std::make_unique<AiExplanationController>(aiExplanationService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AiExplanationController",
                                 GlobalControllers::aiExplanationController.get());

            // User Stack
    auto* userService = config::diConfig().create<application::IUserService*>();
    GlobalControllers::userController = std::make_unique<UserController>(userService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "UserController",
                                 GlobalControllers::userController.get());

            // Dictionary Stack
    auto* dictionaryService = config::diConfig().create<application::IDictionaryService*>();
    GlobalControllers::dictionaryController = std::make_unique<DictionaryController>(dictionaryService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "DictionaryController",
                                 GlobalControllers::dictionaryController.get());

            // Folder Stack
    auto* folderService = config::diConfig().create<application::IFolderService*>();
    GlobalControllers::folderController = std::make_unique<FolderController>(folderService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "FolderController",
                                 GlobalControllers::folderController.get());

            // Library Stack
    auto* libraryService = config::diConfig().create<application::ILibraryService*>();
    GlobalControllers::libraryController = std::make_unique<LibraryController>(libraryService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "LibraryController",
                                 GlobalControllers::libraryController.get());

            // Book Stack
    auto bookService = std::make_unique<application::services::BookService>();
    GlobalControllers::bookController = std::make_unique<BookController>(bookService.get(), libraryService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "BookController",
                                 GlobalControllers::bookController.get());

            // External Book Stack
    auto externalBookService = std::make_unique<application::services::BookService>();
    GlobalControllers::externalBookController = std::make_unique<ExternalBookController>(externalBookService.get());
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "ExternalBookController",
                                 GlobalControllers::externalBookController.get());

            // Free books stack
    auto* freeBooksService = config::diConfig().create<application::IFreeBooksService*>();
    GlobalControllers::freeBooksController = std::make_unique<FreeBooksController>(freeBooksService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "FreeBooksController",
                                 GlobalControllers::freeBooksController.get());

            // Settings Stack
    auto* settingsService = config::diConfig().create<application::ISettingsService*>();
    GlobalControllers::settingsController = std::make_unique<SettingsController>(settingsService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "SettingsController",
                                 GlobalControllers::settingsController.get());

            // Tools Stack
    auto toolsService = std::make_unique<application::services::ToolsService>(libraryService);
    GlobalControllers::toolsController = std::make_unique<ToolsController>(toolsService.get());
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "ToolsController",
                                 GlobalControllers::toolsController.get());

            // Enums
    qmlRegisterUncreatableMetaObject(application::book_operation_status::staticMetaObject, "Librum.controllers",
                                     1, 0, "BookOperationStatus",
                                     "This is an uncreatable enum!");

    qmlRegisterUncreatableMetaObject(application::setting_keys::staticMetaObject, "Librum.controllers",
                                     1, 0, "SettingKeys",
                                     "This is an uncreatable enum!");

    qmlRegisterUncreatableMetaObject(application::setting_groups::staticMetaObject, "Librum.controllers",
                                     1, 0, "SettingGroups",
                                     "This is an uncreatable enum!");

    qmlRegisterUncreatableMetaObject(application::error_codes::staticMetaObject, "Librum.controllers",
                                     1, 0, "ErrorCode",
                                     "This is an uncreatable enum!");

            // Connect the user service's loggedOut with the authentication service's loggedOut
            // signal, since both services need to be able to log out the user.
    QObject::connect(userService, &application::IUserService::logoutUser,
                     authenticationService, &application::IAuthenticationService::logoutUser);


            // Setup login connections
    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     libraryService, &application::ILibraryService::setupUserData);
    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     libraryService, &application::ILibraryService::clearUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     folderService, &application::IFolderService::setupUserData);
    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     folderService, &application::IFolderService::clearUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     freeBooksService, &application::IFreeBooksService::setupUserData);
    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     aiExplanationService, &application::IAiExplanationService::setupUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     aiExplanationService, &application::IAiExplanationService::clearUserData);
    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     freeBooksService, &application::IFreeBooksService::clearUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     userService, &application::IUserService::setupUserData);
    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     userService, &application::IUserService::clearUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     settingsService, &application::ISettingsService::loadUserSettings);
    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     settingsService, &application::ISettingsService::clearUserData);


            // Setup other connections
    QObject::connect(userService, &application::IUserService::bookStorageDataUpdated,
                     libraryService, &application::ILibraryService::updateUsedBookStorage);

    QObject::connect(GlobalControllers::libraryController.get(),
                     &adapters::ILibraryController::downloadedProjectGutenbergIdsReady,
                     GlobalControllers::freeBooksController.get(),
                     &adapters::IFreeBooksController::proccessDownloadedProjectGutenbergIds);

            // Startup
    // QQmlApplicationEngine engine;
    QPointer<QQmlApplicationEngine> engine = new QQmlApplicationEngine();
    QQuickStyle::setStyle("Basic");
    engine->addImportPath("qrc:/modules");
    engine->addImportPath(QCoreApplication::applicationDirPath() + "/src/presentation/qt_tree_view/qml/");
    GlobalControllers::appInfoController->setQmlApplicationEngine(engine);


            // Setup translations
    QSettings settings;
    auto storedLanguage = settings.value("language", QVariant("")).toString();
    if(storedLanguage.isEmpty())
    {
        // If no language was specified in the settings, deduce the system language
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        for(const QString& locale : uiLanguages)
        {
            const QString name = QLocale(locale).name();
            if(GlobalControllers::appInfoController->switchToLanguage(name))
            {
                break;
            }
        }
    }
    else
    {
        GlobalControllers::appInfoController->switchToLanguage(storedLanguage);
    }

    if(app->arguments().size() == 2)
    {
        QString filePath = app->arguments().at(1);
        QFileInfo fileInfo(filePath);
        if(!fileInfo.isFile())
        {
            std::cerr << "Book at: " << filePath.toStdString() << " does not exist.\n";
            QCoreApplication::exit(-1);
        }

        auto setupSuccess = GlobalControllers::externalBookController->setUp(filePath);
        if(!setupSuccess)
        {
            std::cerr << "The file type of: " << filePath.toStdString() << " is not supported.\n";
            QCoreApplication::exit(-1);
        }
        engine->rootContext()->setContextProperty("externalBook", true);
    }
    else {
        engine->rootContext()->setContextProperty("externalBook", false);
    }

    const QUrl url("qrc:/main.qml");
    QObject::connect(
        engine, &QQmlApplicationEngine::objectCreated, app,
        [url](QObject* obj, const QUrl& objUrl)
        {
            if(obj == nullptr && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine->load(url);

    return QPair<QPointer<QApplication>, QPointer<QQmlApplicationEngine>>(app, engine);
}

void setupGlobalSettings()
{
    QSettings settings;
    QString cfgFile = settings.value("serverHost", QVariant("")).toString();
    if(cfgFile.isEmpty())
        settings.setValue("serverHost", "https://api.librumreader.com");

    QString sslSettings = settings.value("selfHosted", QVariant("")).toString();
    if(sslSettings.isEmpty())
        settings.setValue("selfHosted", "false");
}

void loadFont(const QString& path)
{
    int result = QFontDatabase::addApplicationFont(path);
    if(result == -1)
        qWarning() << QString("Loading font file: %1 failed.").arg(path);
}

void setupFonts()
{
    loadFont(":/resources/fonts/SF-Pro-Display-Bold.otf");
    loadFont(":/resources/fonts/SF-Pro-Display-Medium.otf");
    loadFont(":/resources/fonts/SF-Pro-Display-Regular.otf");
    loadFont(":/resources/fonts/SF-Pro-Display-Semibold.otf");

    QFont defaultFont("SF Pro Display");
    defaultFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.1);
    QGuiApplication::setFont(defaultFont);
}
