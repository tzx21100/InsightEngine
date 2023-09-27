#ifndef GAM200_INSIGHT_ENGINE_EDITOR_UTILS_FILEDIALOG_H
#define GAM200_INSIGHT_ENGINE_EDITOR_UTILS_FILEDIALOG_H

#include <string>

namespace IS {

    namespace filedialog {

        /**
         * @brief Opens a file dialog for the user to select a file for opening.
         *
         * This function displays a file dialog that allows the user to select a file for opening.
         *
         * @param filter The file filter to use in the dialog (e.g., "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0").
         * @param relative_path The relative directory path to set as the initial directory for the dialog.
         *                      The dialog will open in this directory initially, allowing the user to navigate from there.
         *
         * @return A string containing the selected file path, or an empty string if the user canceled the operation.
         */
        std::string OpenFile(const char* filter, std::string const& relative_path);

        /**
         * @brief Opens a file dialog for the user to select a location to save a file.
         *
         * This function displays a file dialog that allows the user to specify a location and name for saving a file.
         *
         * @param filter The file filter to use in the dialog (e.g., "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0").
         * @param relative_path The relative directory path to set as the initial directory for the dialog.
         *                      The dialog will open in this directory initially, allowing the user to navigate from there.
         *
         * @return A string containing the selected file path, or an empty string if the user canceled the operation.
         */
        std::string SaveFile(const char* filter, std::string const& relative_path);

        /**
         * @brief Retrieves the current working directory.
         *
         * This function retrieves the current working directory of the program.
         * The path returned uses back slashes ('\') as directory separators.
         *
         * @return A string containing the current working directory.
         */
        std::string GetCurrentWorkingDirectory();


    } // end namespace filedialog

} // end namespace IS

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_UTILS_FILEDIALOG_H
