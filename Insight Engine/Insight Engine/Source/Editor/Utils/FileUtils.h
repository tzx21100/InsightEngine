/*!
 * \file FileUtils.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2451
 * \date 24-10-2023
 * \brief
 * This header file declares the interface for file utility functions using
 * Win32 API.
 *
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/
#ifdef USING_IMGUI
/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_EDITOR_UTILS_FILE_UTILS_H
#define GAM200_INSIGHT_ENGINE_EDITOR_UTILS_FILE_UTILS_H

namespace IS { namespace FileUtils {

    /*!
    * \brief Opens a file from its default application.
    *
    * This function opens a file from its default application.
    *
    * \param filepath The path to the file.
    * \param directory The relative directory path to the file.
    */
    void OpenFileFromDefaultApp(const char* filepath, const char* directory = nullptr);

    /*!
    * \brief Opens a file dialog for the user to select a file for opening.
    *
    * This function displays a file dialog that allows the user to select a file for opening.
    *
    * \param filter The file filter to use in the dialog (e.g., "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0").
    * \param relative_path The relative directory path to set as the initial directory for the dialog.
    *                      The dialog will open in this directory initially, allowing the user to navigate from there.
    *
    * \return A string containing the selected file path, or an empty string if the user canceled the operation.
    */
    std::string OpenFile(const char* filter, std::string const& relative_path);

    /*!
    * \brief Opens a file dialog for the user to select a location to save a file.
    *
    * This function displays a file dialog that allows the user to specify a location and name for saving a file.
    *
    * \param filter The file filter to use in the dialog (e.g., "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0").
    * \param relative_path The relative directory path to set as the initial directory for the dialog.
    *                      The dialog will open in this directory initially, allowing the user to navigate from there.
    *
    * \return A string containing the selected file path, or an empty string if the user canceled the operation.
    */
    std::string SaveFile(const char* filter, std::string const& relative_path);

    /*!
    * \brief Retrieves the current working directory.
    *
    * This function retrieves the current working directory of the program.
    * The path returned uses back slashes ('\') as directory separators.
    *
    * \return A string containing the current working directory.
    */
    std::string GetCurrentWorkingDirectory();

    /*!
    * \brief Open a script.
    */
    void OpenScript();

    /*!
    * \brief Open and get a script.
    *
    * \return filepath to the script to be changed into.
    */
    std::string OpenAndGetScript();

    /*!
    * \brief Check if file exists.
    *
    * \param filename Filepath to the file to check for. (ONLY ASCII)
    * \return True if file exists, false otherwise.
    */
    bool FileExists(std::string const& filename);

    /*!
    * \brief Makes a copy of a file.
    *
    * \param source_filepath The filepath to the source file. (ONLY ASCII)
    * \param destination_directory The filepath to the destination directory. (ONLY ASCII)
    * \return True if operation succeeded, false otherwise.
    */
    bool FileMakeCopy(std::string const& source_filepath, std::string const& destination_directory);

    /*!
    * \brief Deletes file specified.
    *
    * \param filename Filepath to the file to delete. (ONLY ASCII)
    * \return True if operation succeeded, false otherwise.
    */
    bool FileDelete(std::string const& filename);

} } // end namespace IS::FileUtils

#endif // !GAM200_INSIGHT_ENGINE_EDITOR_UTILS_FILE_UTILS_H


#endif // USING_IMGUI
