#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <memory>
#include "authentication_controller.hpp"
#include "app_info_controller.hpp"
#include "ai_explanation_controller.hpp"
#include "user_controller.hpp"
#include "dictionary_controller.hpp"
#include "folder_controller.hpp"
#include "library_controller.hpp"
#include "book_controller.hpp"
#include "external_book_controller.hpp"
#include "free_books_controller.hpp"
#include "settings_controller.hpp"
#include "tools_controller.hpp"


using namespace adapters::controllers;

class GlobalControllers {
public:
    static std::unique_ptr<AuthenticationController> authenticationController;
    static std::unique_ptr<AppInfoController> appInfoController;
    static std::unique_ptr<AiExplanationController> aiExplanationController;
    static std::unique_ptr<UserController> userController;
    static std::unique_ptr<DictionaryController> dictionaryController;
    static std::unique_ptr<FolderController> folderController;
    static std::unique_ptr<LibraryController> libraryController;
    static std::unique_ptr<BookController> bookController;
    static std::unique_ptr<ExternalBookController> externalBookController;
    static std::unique_ptr<FreeBooksController> freeBooksController;
    static std::unique_ptr<SettingsController> settingsController;
    static std::unique_ptr<ToolsController> toolsController;
};

#endif // GLOBALS_HPP
