/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

Component* createMainContentComponent();

using juce::File;
using juce::FileBrowserComponent;
using juce::CodeEditorComponent;

//==============================================================================
class SoundServerApplication  : public JUCEApplication
{
public:
    //==============================================================================
    SoundServerApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..

        mainWindow = new MainWindow (getApplicationName());
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainContentComponent class.
    */
    class MainWindow    : public DocumentWindow, public MenuBarModel
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (createMainContentComponent(), true);
            setResizable (true, true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
			setMenuBar(this);

			logComp = new CodeEditorComponent(logDoc, nullptr);

			setContentComponent(logComp);
        }

		~MainWindow()
		{
			setMenuBar(nullptr);
			delete logComp;
		}

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

		virtual StringArray getMenuBarNames()
		{
			const char* const names[] = { "File", nullptr };

			return StringArray(names);
		}

		virtual PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName)
		{
			PopupMenu menu;

			if (topLevelMenuIndex == 0)
			{
				menu.addItem(200, "Open Midi File", true);
			}

			return menu;
		}

		virtual void menuItemSelected(int menuItemID, int)
		{
			if (menuItemID == 200)
			{
				WildcardFileFilter wildcardFilter("*.mid", String::empty, "Midi files");
				FileBrowserComponent browser(FileBrowserComponent::canSelectFiles | FileBrowserComponent::openMode, 
					lastOpenedFile.exists() ? lastOpenedFile : File(String("C:\\Users\\GeorgeKrueger\\Documents")), &wildcardFilter, nullptr);
				FileChooserDialogBox dialogBox("Open a midi file",
					"Please choose a midi file to open...",
					browser,
					false,
					Colours::lightgrey);

				if (dialogBox.show())
				{
					File selectedFile = browser.getSelectedFile(0);
					lastOpenedFile = selectedFile;
					FileInputStream fileStream(selectedFile);
					MidiFile midiFile;
					midiFile.readFrom(fileStream);
					int numTracks = midiFile.getNumTracks();
					String msg;
					msg << "Num Tracks: " << numTracks << "\n";
					log(msg);

					for (int i = 0; i < numTracks; ++i)
					{
						const MidiMessageSequence* msgSeq = midiFile.getTrack(i);
					}
				}
			}
		}

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)

	private:
		CodeDocument logDoc;
		CodeEditorComponent* logComp;
		File lastOpenedFile;

		void log(const String& text)
		{
			logComp->moveCaretToEnd(false);
			logComp->insertTextAtCaret(text);
		}
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (SoundServerApplication)
