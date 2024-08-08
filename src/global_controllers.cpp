#include "global_controllers.h"

std::unique_ptr<AuthenticationController> GlobalControllers::authenticationController = nullptr;
std::unique_ptr<AppInfoController> GlobalControllers::appInfoController = nullptr;
std::unique_ptr<AiExplanationController> GlobalControllers::aiExplanationController = nullptr;
std::unique_ptr<UserController> GlobalControllers::userController = nullptr;
std::unique_ptr<DictionaryController> GlobalControllers::dictionaryController = nullptr;
std::unique_ptr<FolderController> GlobalControllers::folderController = nullptr;
std::unique_ptr<LibraryController> GlobalControllers::libraryController = nullptr;
std::unique_ptr<BookController> GlobalControllers::bookController = nullptr;
std::unique_ptr<ExternalBookController> GlobalControllers::externalBookController = nullptr;
std::unique_ptr<FreeBooksController> GlobalControllers::freeBooksController = nullptr;
std::unique_ptr<SettingsController> GlobalControllers::settingsController = nullptr;
std::unique_ptr<ToolsController> GlobalControllers::toolsController = nullptr;

