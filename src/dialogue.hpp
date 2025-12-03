#pragma once

#include <constants.hpp>
#include <fstream>
#include <list>
#include <map>
#include <raylib.h>
#include <resources.hpp>
#include <sstream>
#include <string>
using namespace std;
using namespace GameConstants;
using namespace Resources::MenuResource;
using namespace Resources::FontResource;
using namespace Resources::AvatarResource;
using namespace Resources::DialogueResource;
using namespace Resources::DialogueBoxResource;

struct choice
{
	int choiceID;
	string choiceText;
	string Filename;
};

class Dialogue
{
  private:
	string Speaker;
	string DialogueText;
	string AvatarPath;

  public:
	Dialogue(const string &speaker, const string &dialogueText, const string &avatarPath)
		: Speaker(speaker), DialogueText(dialogueText), AvatarPath(avatarPath)
	{
	}

	string getSpeaker() const { return Speaker; }
	string getDialogueText() const { return DialogueText; }
	string getAvatarPath() const { return AvatarPath; }
};

class StoryDialogues
{
  private:
	list<Dialogue> dialogues;
	list<choice> choices;
	string endDialogueFile;

  public:
	void addDialogue(const Dialogue &d) { dialogues.push_back(d); }
	void addChoice(const choice &c) { choices.push_back(c); }
	void setEndDialogueFile(const string &file) { endDialogueFile = file; }

	const list<Dialogue> &getDialogues() const { return dialogues; }
	const list<choice> &getChoices() const { return choices; }
	string getEndDialogueFile() const { return endDialogueFile; }

	void clear()
	{
		dialogues.clear();
		choices.clear();
		endDialogueFile = "";
	}
};

class DialogueParser
{
  private:
	string Filename;

	string trim(const string &str)
	{
		size_t first = str.find_first_not_of(" \t\n\r");
		if (first == string::npos)
			return "";
		size_t last = str.find_last_not_of(" \t\n\r");
		return str.substr(first, (last - first + 1));
	}

  public:
	DialogueParser(string filename) : Filename(filename) {}

	StoryDialogues FileParser()
	{
		StoryDialogues story;
		ifstream ifs(Filename);

		if (!ifs.is_open())
		{
			printf("Failed to open file: %s\n", Filename.c_str());
			return story;
		}

		string line;
		string currentSpeaker = "";
		string dialogueText = "";
		string avatarPath = "";
		bool readingDialogue = false;
		int choiceCounter = 0;

		while (getline(ifs, line))
		{
			line = trim(line);
			if (line.empty())
				continue;

			// ===== SPEAKER TAG [SpeakerName] =====
			if (line[0] == '[' && line[line.length() - 1] == ']')
			{
				// Save previous dialogue
				if (readingDialogue && !currentSpeaker.empty() && !dialogueText.empty())
				{
					story.addDialogue(Dialogue(currentSpeaker, dialogueText, avatarPath));
					dialogueText = "";
				}

				// Extract speaker name
				currentSpeaker = line.substr(1, line.length() - 2);

				// Check for special tags
				if (currentSpeaker == "End Dialogue")
				{
					readingDialogue = false;
					continue;
				}
				if (currentSpeaker == "Choice")
				{
					readingDialogue = false;
					continue;
				}

				// Set avatar path
				if (currentSpeaker == "MC")
					avatarPath = KNIGHT_IMAGE;
				else if (currentSpeaker == "Kaelen")
					avatarPath = KAELEN_IMAGE;
				else if (currentSpeaker == "Alistair")
					avatarPath = ALISTAIR_IMAGE;
				else if (currentSpeaker == "Edric")
					avatarPath = EDRIC_IMAGE;
				else
					avatarPath = UNKNOWN_IMAGE;

				readingDialogue = true;
			}
			// ===== CHOICE LINE : text -> file =====
			else if (line[0] == ':')
			{
				size_t arrowPos = line.find("->");
				if (arrowPos != string::npos)
				{
					choice c;
					c.choiceID = choiceCounter++;
					c.choiceText = trim(line.substr(1, arrowPos - 1));
					c.Filename = trim(line.substr(arrowPos + 2));
					story.addChoice(c);
				}
			}
			// ===== END DIALOGUE FILE -> file =====
			else if (line.find("->") == 0)
			{
				story.setEndDialogueFile(trim(line.substr(2)));
			}
			// ===== REGULAR DIALOGUE TEXT =====
			else if (readingDialogue)
			{
				if (!dialogueText.empty())
					dialogueText += " ";
				dialogueText += line;
			}
		}

		// Save last dialogue
		if (readingDialogue && !currentSpeaker.empty() && !dialogueText.empty())
		{
			story.addDialogue(Dialogue(currentSpeaker, dialogueText, avatarPath));
		}

		ifs.close();
		return story;
	}
};

class DialogueBox
{
  private:
	// ===== CONSTANTS =====
	static constexpr float CHARS_PER_SECOND = 30.0f;
	static constexpr int FONT_SIZE = 30;

	// ===== TEXTURES & FONTS =====
	Font font;
	Texture2D boxTexture;
	Texture2D backgroundTexture;
	map<string, Texture2D> avatarCache;
	bool fontLoaded;
	bool boxTextureLoaded;
	bool backgroundTextureLoaded;

	// ===== LAYOUT =====
	Rectangle boxDest;
	Rectangle avatarDest;
	Vector2 textPos;
	Vector2 namePos;
	Color fontColor;
	Color nameColor;

	// ===== DIALOGUE DATA =====
	list<Dialogue> dialogues;
	list<choice> choices;
	list<Dialogue>::iterator currentDialogue;

	// ===== STATE =====
	bool isActive;
	bool showingChoices;
	string nextDialogueFile;	// For immediate loading (choices)
	string pendingDialogueFile; // For delayed loading ([End Dialogue])

	// ===== ANIMATION =====
	float charTimer;
	int visibleChars;
	string currentWrappedText;
	bool textFullyRevealed;

	// ===== HELPER FUNCTIONS =====

	Texture2D GetAvatarTexture(const string &path)
	{
		// Check cache
		if (avatarCache.find(path) != avatarCache.end())
		{
			return avatarCache[path];
		}

		// Load and process
		Image avatar = LoadImage(path.c_str());
		if (avatar.data != nullptr)
		{
			Color white = {255, 255, 255, 255};
			Color transparent = {0, 0, 0, 0};
			ImageColorReplace(&avatar, white, transparent);

			Texture2D tex = LoadTextureFromImage(avatar);
			UnloadImage(avatar);

			if (tex.id != 0)
			{
				avatarCache[path] = tex;
				return tex;
			}
		}

		return {0};
	}

	string GetVisibleText(const string &text, int charCount)
	{
		if (charCount >= (int)text.length())
			return text;
		return text.substr(0, charCount);
	}

	string WrapText(const string &text, Font font, float fontSize, float maxWidth)
	{
		string result;
		string currentLine;
		istringstream words(text);
		string word;

		while (words >> word)
		{
			string testLine = currentLine.empty() ? word : currentLine + " " + word;
			float lineWidth = MeasureTextEx(font, testLine.c_str(), fontSize + 30, 2).x;

			if (lineWidth > maxWidth)
			{
				if (!currentLine.empty())
				{
					result += currentLine + "\n";
					currentLine = word;
				}
				else
				{
					result += word + "\n";
					currentLine = "";
				}
			}
			else
			{
				currentLine = testLine;
			}
		}

		if (!currentLine.empty())
			result += currentLine;
		return result;
	}

  public:
	// ===== CONSTRUCTOR =====
	DialogueBox()
	{
		// State
		isActive = false;
		showingChoices = false;
		fontLoaded = false;
		boxTextureLoaded = false;
		backgroundTextureLoaded = false;
		nextDialogueFile = "";
		pendingDialogueFile = "";

		// Animation
		charTimer = 0.0f;
		visibleChars = 0;
		textFullyRevealed = false;

		// Layout
		boxDest = {500.0f, 720.0f, 800.0f, 300.0f};
		avatarDest = {532.0f, 770.0f, 125.0f, 160.0f};
		textPos = {725.0f, 810.0f};
		namePos = {720.0f, 750.0f};
		fontColor = PURPLE;
		nameColor = BLACK;
	}

	// ===== DESTRUCTOR =====
	~DialogueBox()
	{
		if (fontLoaded)
			UnloadFont(font);
		if (boxTextureLoaded)
			UnloadTexture(boxTexture);
		if (backgroundTextureLoaded)
			UnloadTexture(backgroundTexture);

		for (auto &pair : avatarCache)
		{
			UnloadTexture(pair.second);
		}
		avatarCache.clear();
	}

	// ===== INITIALIZATION =====
	void Initialize()
	{
		if (!fontLoaded)
		{
			font = LoadFont(IMFELL_ENGLISHSC_REGULAR.c_str());
			fontLoaded = true;
		}
		if (!boxTextureLoaded)
		{
			// Load image first
			Image boxImage = LoadImage(DB_IMAGE.c_str());

			if (boxImage.data != nullptr)
			{
				// Replace black with transparent
				Color black = {0, 0, 0, 255};
				Color transparent = {0, 0, 0, 0};
				ImageColorReplace(&boxImage, black, transparent);

				// Load texture from processed image
				boxTexture = LoadTextureFromImage(boxImage);
				UnloadImage(boxImage);
				boxTextureLoaded = true;
			}
		}
		if (!backgroundTextureLoaded)
		{
			backgroundTexture = LoadTexture(DB_BG_IMAGE.c_str());
			backgroundTextureLoaded = (backgroundTexture.id != 0);
		}
	}
	// ===== START DIALOGUE =====
	void StartDialogue(const string &filename)
	{
		DialogueParser parser(filename);
		StoryDialogues story = parser.FileParser();

		dialogues = story.getDialogues();
		choices = story.getChoices();
		pendingDialogueFile = story.getEndDialogueFile();
		nextDialogueFile = "";

		if (!dialogues.empty())
		{
			currentDialogue = dialogues.begin();
			isActive = true;
			showingChoices = false;

			float wrapWidth = boxDest.width - 40;
			currentWrappedText
				= WrapText(currentDialogue->getDialogueText(), font, FONT_SIZE, wrapWidth);
			charTimer = 0.0f;
			visibleChars = 0;
			textFullyRevealed = false;
		}
		else
		{
			isActive = false;
		}
	}

	// ===== UPDATE =====
	void Update()
	{
		if (!isActive)
			return;

		// Animate text
		if (!textFullyRevealed && !showingChoices)
		{
			charTimer += GetFrameTime();
			float charsToAdd = charTimer * CHARS_PER_SECOND;

			if (charsToAdd >= 1.0f)
			{
				visibleChars += (int)charsToAdd;
				charTimer -= (int)charsToAdd / CHARS_PER_SECOND;

				if (visibleChars >= (int)currentWrappedText.length())
				{
					visibleChars = currentWrappedText.length();
					textFullyRevealed = true;
				}
			}
		}

		// Handle choice selection
		if (showingChoices)
		{
			int choiceCount = 0;
			for (const auto &c : choices)
			{
				int keyToPress = KEY_ONE + choiceCount;
				if (IsKeyPressed(keyToPress) && keyToPress <= KEY_NINE)
				{
					nextDialogueFile = c.Filename;
					isActive = false;
					showingChoices = false;
					return;
				}
				choiceCount++;
				if (choiceCount >= 9)
					break;
			}
		}
		// Handle ENTER key
		else if (IsKeyPressed(KEY_ENTER))
		{
			if (!textFullyRevealed)
			{
				// Skip animation
				visibleChars = currentWrappedText.length();
				textFullyRevealed = true;
			}
			else
			{
				// Advance dialogue
				currentDialogue++;

				if (currentDialogue == dialogues.end())
				{
					if (!choices.empty())
					{
						showingChoices = true;
						currentDialogue--;
					}
					else
					{
						isActive = false;
					}
				}
				else
				{
					float wrapWidth = boxDest.width - 40;
					currentWrappedText
						= WrapText(currentDialogue->getDialogueText(), font, FONT_SIZE, wrapWidth);
					charTimer = 0.0f;
					visibleChars = 0;
					textFullyRevealed = false;
				}
			}
		}
	}

	// ===== DRAW =====
	void Draw()
	{
		if (!isActive || currentDialogue == dialogues.end())
			return;

		// Draw full screen background
		if (backgroundTextureLoaded)
		{
			Rectangle bgSource
				= {0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height};
			Rectangle bgDest = {0, 0, 1920, 1080};
			DrawTexturePro(backgroundTexture, bgSource, bgDest, {0, 0}, 0.0f, WHITE);
		}

		// Get avatar
		Texture2D avatarTexture = GetAvatarTexture(currentDialogue->getAvatarPath());

		// Draw dialogue box
		DrawTexturePro(boxTexture, {0, 0, (float)boxTexture.width, (float)boxTexture.height},
					   boxDest, {0, 0}, 0.0f, WHITE);

		// Draw avatar
		if (avatarTexture.id != 0)
		{
			DrawTexturePro(avatarTexture,
						   {0, 0, (float)avatarTexture.width, (float)avatarTexture.height},
						   avatarDest, {0, 0}, 0.0f, WHITE);
		}

		// Draw speaker name
		DrawTextEx(font, currentDialogue->getSpeaker().c_str(), namePos, FONT_SIZE + 15, 2,
				   nameColor);

		// Draw dialogue text
		string visibleText = GetVisibleText(currentWrappedText, visibleChars);
		DrawTextEx(font, visibleText.c_str(), textPos, FONT_SIZE, 2, fontColor);

		// Draw choices or prompt
		if (showingChoices)
		{
			int yOffset = 0;
			int choiceNum = 1;
			for (const auto &c : choices)
			{
				Vector2 choicePos = {725, 900 + (float)yOffset};
				string choiceDisplay = to_string(choiceNum) + ". " + c.choiceText;
				DrawTextEx(font, choiceDisplay.c_str(), choicePos, FONT_SIZE - 5, 2, GOLD);
				yOffset += 40;
				choiceNum++;
			}
		}
		else
		{
			if (textFullyRevealed)
			{
				DrawTextEx(font, "Press ENTER...", {1150, 980}, FONT_SIZE - 10, 2, GRAY);
			}
			else
			{
				DrawTextEx(font, "Press ENTER to skip", {1100, 980}, FONT_SIZE - 10, 2, DARKGRAY);
			}
		}
		// Draw dialogue box on top
		const char *hint = "Press S to skip dialogue.";
		float hintSize = 30;
		Vector2 hintDim = MeasureTextEx(font, hint, hintSize, 2);
		DrawTextEx(font, hint, {(SCREEN_WIDTH - hintDim.x) / 2, 150}, hintSize, 2, LIGHTGRAY);
	}

	// ===== GETTERS =====
	bool IsActive() const { return isActive; }

	bool HasNextDialogue() const { return !nextDialogueFile.empty(); }
	string GetNextDialogueFile()
	{
		string file = nextDialogueFile;
		nextDialogueFile = "";
		return file;
	}

	bool HasPendingDialogue() const { return !pendingDialogueFile.empty(); }
	string GetPendingDialogueFile()
	{
		string file = pendingDialogueFile;
		pendingDialogueFile = "";
		return file;
	}

	// ===== MANUAL CONTROL =====
	void EndDialogue()
	{
		isActive = false;
		showingChoices = false;
	}

	void Reset()
	{
		// ---- STATE ----
		isActive = false;
		showingChoices = false;

		// ---- DIALOGUE DATA ----
		dialogues.clear();
		choices.clear();
		nextDialogueFile.clear();
		pendingDialogueFile.clear();

		// Reset iterator safely
		currentDialogue = dialogues.end();

		// ---- ANIMATION ----
		charTimer = 0.0f;
		visibleChars = 0;
		currentWrappedText.clear();
		textFullyRevealed = false;
	}
};
