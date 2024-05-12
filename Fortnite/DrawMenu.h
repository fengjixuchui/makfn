#pragma once
#include "Wrapper.h"

namespace Framework
{
	namespace ColorPicker
	{
		float ColorAddWhite(float current, float white)
		{
			current += white;

			if (current >= 255.f) return 255.f;

			return current;
		}

		static FLinearColor Rainbow(float speed = 0.1f)
		{
			static unsigned int cnt = 0;

			FLinearColor color = FLinearColor(
				UKismetMathLibrary::sin(speed * cnt + 0) * 0.5 + 0.5,
				UKismetMathLibrary::sin(speed * cnt + 2) * 0.5 + 0.5,
				UKismetMathLibrary::sin(speed * cnt + 4) * 0.5 + 0.5,
				1);

			if (cnt++ >= (unsigned int)-1) cnt = 0;

			return color;
		}
	}

	class GUI
	{
	public:
		void Initialize()
		{
			this->BackgroundColor = FLinearColor(0.1510f, 0.1392f, 0.1392f, 1.0000f);
			this->MainColor = FLinearColor(0.0578f, 0.0578f, 0.0578f, 1.0000f);
			this->FrameColor = FLinearColor(0.1078f, 0.1078f, 0.1078f, 1.0000f);
			this->FrameHoveredColor = FLinearColor(0.1366f, 0.1288f, 0.1288f, 1.0000f);
			this->FramePressedColor = FLinearColor(0.1078f, 0.1078f, 0.1078f, 1.0000f);
			this->TextColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

			this->Position = FVector2D(200, 200);
			this->Size = FVector2D(650, 385);
		}

		bool CheckHovered(FVector2D pos, FVector2D Size)
		{
			if (this->Cursor.X > pos.X && this->Cursor.Y > pos.Y)
				if (this->Cursor.X < pos.X + Size.X && this->Cursor.Y < pos.Y + Size.Y)
					return true;

			return false;
		}

		void Combobox(FString Content, bool* Active, int* Value, FString* Arguments, int NumArguments)
		{
			this->Offset.Y += 5;

			FVector2D Current = this->Position + this->Offset;
			FVector2D Size = FVector2D(150, 24);

			bool Hovered = this->CheckHovered(Current, Size);
			bool Clicked = this->LeftMouseClicked && Hovered;
			bool AnyHovered = Hovered;

			if (Clicked)
			{
				if (*Active)
					*Active = false;
				else if (!this->TotalOpenPopups)
					*Active = true;
			}

			Wrapper::RectFilled(Current, Size, this->FrameColor);
			Wrapper::Rect(Current, Size, FLinearColor(0.f, 0.f, 0.f, 1.f), 1.f);

			Wrapper::Text(Arguments[*Value], FVector2D(Current.X + (Size.X / 2) - 2, Current.Y + 3), this->TextColor, true, false);
			Wrapper::Text(Content, FVector2D(Current.X + Size.X + 5, Current.Y + 3), this->TextColor, false, false);

			if (*Active)
			{
				this->TotalOpenPopups += 1;

				FVector2D SelectionStart = FVector2D(Current.X, Size.Y);

				for (int i = 0; i < NumArguments; i++)
				{
					if (i == *Value) continue;

					Current.Y += Size.Y;

					bool ItemHovered = this->CheckHovered(Current, Size);
					bool ItemClicked = ItemHovered && this->LeftMouseClicked;
					FLinearColor ItemColor = this->FrameColor;

					if (ItemHovered && !AnyHovered)
						AnyHovered = true;

					if (ItemClicked)
						*Value = i;

					if (ItemHovered)
						ItemColor = this->FrameHoveredColor;
					else if (*Value == i)
						ItemColor = this->FramePressedColor;

					Wrapper::RectFilled(Current, Size, ItemColor, true);
					Wrapper::Rect(Current, Size, FLinearColor(0.f, 0.f, 0.f, 1.f), 1.f, true);

					Wrapper::Text(Arguments[i], FVector2D(Current.X + (Size.X / 2), Current.Y + 2), this->TextColor, true, false, true);
				}
			}

			bool ClickedSomewhereElse = !AnyHovered && this->LeftMouseClicked;
			if (ClickedSomewhereElse && *Active)
				*Active = false;

			this->Offset.Y += 26; // 28
		}

		void KeyInput(FString Content, FString* String, bool* Active)
		{
			this->Offset.Y += 6;

			FVector2D Current = this->Position + this->Offset;
			FVector2D Size = FVector2D(9, 9);

			bool Hovered = this->CheckHovered(FVector2D(Current.X - 1, Current.Y - 1), FVector2D(Size.X + 80, Size.Y));
			bool BigHovered = Hovered || (*Active && this->CheckHovered(Current, FVector2D(170, 115)));
			bool Clicked = Hovered && this->LeftMouseClicked;
			bool ClickedSomewhereElse = !BigHovered && this->LeftMouseClicked;

			Wrapper::RectFilled(Current, Size, this->FrameColor);
			Wrapper::Rect(Current, Size, FLinearColor(0.f, 0.f, 0.f, 1.f), 1.f);

			Wrapper::Text(Content, FVector2D(Current.X + Size.X + 6, Current.Y - 3), this->TextColor, false, false);

			if (Clicked)
			{
				if (*Active)
					*Active = false;
				else if (!this->TotalOpenPopups)
					*Active = true;
			}

			if (ClickedSomewhereElse && *Active)
				*Active = false;

			if (*Active)
			{
				this->TotalOpenPopups += 1;

				Current = FVector2D(Current.X + Size.X, Current.Y + Size.Y);
				double CurrentXBackup = Current.X;

				FString CurrentText = {};
				FString Keybinds[] = { L"W", L"A", L"S", L"D", L"" };

				for (int ci = 0; ci < sizeof(Keybinds) / sizeof(FString); ci++)
				{
					FString CurrentKey = Keybinds[ci];

					FKey CurrentFKey = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(CurrentKey) }, 0 };
					if (Variables::PlayerController->WasInputKeyJustPressed(CurrentFKey))
					{
						CurrentText += CurrentKey;
					}
				}

				Wrapper::RectFilled(Current, FVector2D(20, 60), FLinearColor(0.f, 0.f, 0.f, 1.f), true);
				Wrapper::Text(CurrentText, FVector2D(Current.X + 20 + 6, 60 - 3), this->TextColor, false, false);

				Current.X += 10;

				bool this_color_hovered = this->CheckHovered(Current, FVector2D(11, 11));
				bool this_color_clicked = this->LeftMouseClicked && this_color_hovered;

				if (this_color_clicked)
				{
					*String = CurrentText;
					*Active = false;
				}

				Wrapper::RectFilled(Current, FVector2D(10, 10), this->FrameColor, true);
				Wrapper::Text(L"Enter", FVector2D(Current.X + 10 + 6, 10 - 3), this->TextColor, false, false);
			}

			this->Offset.Y += 13;
		}

		void ColorPicker(FString Content, FLinearColor* Color, bool* Active)
		{
			this->Offset.Y += 6;

			FVector2D Current = this->Position + this->Offset;
			FVector2D Size = FVector2D(9, 9);

			bool Hovered = this->CheckHovered(FVector2D(Current.X - 1, Current.Y - 1), FVector2D(Size.X + 80, Size.Y));
			bool BigHovered = Hovered || (*Active && this->CheckHovered(Current, FVector2D(170, 115)));
			bool Clicked = Hovered && this->LeftMouseClicked;
			bool ClickedSomewhereElse = !BigHovered && this->LeftMouseClicked;

			Wrapper::RectFilled(Current, Size, *Color);
			Wrapper::Rect(Current, Size, FLinearColor(0.f, 0.f, 0.f, 1.f), 1.f);

			Wrapper::Text(Content, FVector2D(Current.X + Size.X + 6, Current.Y - 3), this->TextColor, false, false);

			if (Clicked)
			{
				if (*Active)
					*Active = false;
				else if (!this->TotalOpenPopups)
					*Active = true;
			}

			if (ClickedSomewhereElse && *Active)
				*Active = false;

			if (*Active)
			{
				this->TotalOpenPopups += 1;

				Current = FVector2D(Current.X + Size.X, Current.Y + Size.Y);
				double CurrentXBackup = Current.X;

				//Top -> Bottom (White)
				for (float c = 0; c < 10.f; c++)
				{
					float c_percentage = c / 10.f;

					float c_hx = 200.f * c_percentage;

					//Red -> Green
					for (float i = 0; i < 5.f; i++)
					{
						float percentage = i / 5.f;

						float hx = 255.f * percentage;

						float red = 255.f - hx;
						float green = 255.f - red;
						float blue = 0.f;

						red = ColorPicker::ColorAddWhite(red, c_hx);
						green = ColorPicker::ColorAddWhite(green, c_hx);
						blue = ColorPicker::ColorAddWhite(blue, c_hx);

						FLinearColor converted_color = FLinearColor(red / 255.f, green / 255.f, blue / 255.f, 1.f);

						bool this_color_hovered = this->CheckHovered(Current, FVector2D(11, 11));
						bool this_color_clicked = this->LeftMouseClicked && this_color_hovered;

						if (this_color_clicked)
						{
							*Color = converted_color;
							*Active = false;
						}

						Wrapper::RectFilled(Current, FVector2D(10, 10), converted_color, true);
						Current.X += 10;
					}

					//Green -> Blue
					for (float i = 0; i < 5.f; i++)
					{
						float percentage = i / 5.f;

						float hx = 255.f * percentage;

						float red = 0.f;
						float green = 255.f - hx;
						float blue = 255.f - green;

						red = ColorPicker::ColorAddWhite(red, c_hx);
						green = ColorPicker::ColorAddWhite(green, c_hx);
						blue = ColorPicker::ColorAddWhite(blue, c_hx);

						FLinearColor converted_color = FLinearColor(red / 255.f, green / 255.f, blue / 255.f, 1.f);

						bool this_color_hovered = this->CheckHovered(Current, FVector2D(11, 11));
						bool this_color_clicked = this->LeftMouseClicked && this_color_hovered;

						if (this_color_clicked)
						{
							*Color = converted_color;
							*Active = false;
						}

						Wrapper::RectFilled(Current, FVector2D(10, 10), converted_color, true);
						Current.X += 10;
					}

					//Blue -> Red
					for (float i = 0; i < 5.f; i++)
					{
						float percentage = i / 5.f;

						float hx = 255.f * percentage;

						float green = 0.f;
						float blue = 255.f - hx;
						float red = 255.f - blue;

						red = ColorPicker::ColorAddWhite(red, c_hx);
						green = ColorPicker::ColorAddWhite(green, c_hx);
						blue = ColorPicker::ColorAddWhite(blue, c_hx);

						FLinearColor converted_color = FLinearColor(red / 255.f, green / 255.f, blue / 255.f, 1.f);

						bool this_color_hovered = this->CheckHovered(Current, FVector2D(11, 11));
						bool this_color_clicked = this_color_hovered && this->LeftMouseClicked;

						if (this_color_clicked)
						{
							*Color = converted_color;
							*Active = false;
						}

						Wrapper::RectFilled(Current, FVector2D(10, 10), converted_color, true);
						Current.X += 10;
					}

					Current.X = CurrentXBackup;
					Current.Y += 10;
				}

				Current.X = CurrentXBackup;

				//White -> Black
				for (float i = 0; i < 15.f; i++)
				{
					float percentage = i / 15.f;

					float hx = 255.f * percentage;

					float green = 255.f - hx;
					float blue = 255.f - hx;
					float red = 255.f - hx;

					FLinearColor converted_color = FLinearColor(red / 255.f, green / 255.f, blue / 255.f, 1.f);

					bool this_color_hovered = this->CheckHovered(Current, FVector2D(11, 11));
					bool this_color_clicked = this->LeftMouseClicked && this_color_hovered;

					if (this_color_clicked)
					{
						*Color = converted_color;
						*Active = false;
					}

					Wrapper::RectFilled(Current, FVector2D(10, 10), converted_color, true);
					Current.X += 10;
				}
			}

			this->Offset.Y += 13;
		}

		void Checkbox(FString Content, bool* Option)
		{
			this->Offset.Y += 6;

			FVector2D Current = this->Position + this->Offset;
			FVector2D Size = FVector2D(9, 9);
			FLinearColor RenderColor;

			bool Hovered = this->CheckHovered(FVector2D(Current.X - 1, Current.Y - 1), FVector2D(Size.X + 100, Size.Y + 1));
			bool Clicked = this->LeftMouseClicked && Hovered;

			if (*Option)
				RenderColor = this->MainColor;
			else if (Hovered)
				RenderColor = this->FrameHoveredColor;
			else
				RenderColor = this->FrameColor;

			if (Clicked && !TotalOpenPopups)
				*Option = !*Option;

			Wrapper::RectFilled(Current, Size, RenderColor);
			Wrapper::Rect(Current, Size, FLinearColor(0.f, 0.f, 0.f, 1.f), 1.f);

			Wrapper::Text(Content, FVector2D(Current.X + Size.X + 6, Current.Y - 4), this->TextColor, false, false);

			this->Offset.Y += 13;
		}

		void Slider(FString Content, double* Value, double Min, double Max)
		{
			this->Offset.Y += 6;

			FVector2D Current = this->Position + this->Offset;
			FVector2D Size = FVector2D(125, 12);
			FLinearColor RenderColor;

			bool Hovered = this->CheckHovered(Current, FVector2D(Size.X + 2, Size.Y + 2));
			bool Down = this->LeftMouseDown && Hovered;

			if (Hovered && Down && !this->TotalOpenPopups)
			{
				*Value = ((this->Cursor.X - Current.X) * ((Max - Min) / Size.X)) + Min;
				if (*Value < Min) *Value = Min;
				if (*Value > Max) *Value = Max;
			}

			if (Down)
				RenderColor = this->FramePressedColor;
			else if (Hovered)
				RenderColor = this->FrameHoveredColor;
			else
				RenderColor = this->FrameColor;

			Wrapper::RectFilled(FVector2D(Current.X + 1, Current.Y + 1), FVector2D(Size.X, Size.Y - 1), RenderColor);
			Wrapper::Rect(FVector2D(Current.X + 1, Current.Y + 1), FVector2D(Size.X, Size.Y - 1), FLinearColor(0.f, 0.f, 0.f, 1.f), 1.f);

			double Percent = Size.X / (Max - Min);
			FVector2D Point = FVector2D(Percent * (*Value - Min), Size.Y);
			Wrapper::RectFilled(FVector2D(Current.X + 1, Current.Y + 1), FVector2D(Point.X, Size.Y), this->MainColor);

			FString ConvertedContent = UKismetStringLibrary::BuildString_Double(Content, FString(L" "), int(*Value), FString(L""));
			Wrapper::Text(ConvertedContent, FVector2D(Current.X + Size.X + 7, Current.Y - 2), this->TextColor, false, false, false);

			this->Offset.Y += 14;
		}

		bool MenuButton(FString Content, bool Active)
		{
			FVector2D Current = FVector2D(this->Position.X + this->Offset.X, this->Position.Y + this->Offset.Y);
			FVector2D Size = FVector2D(117, 25);
			FLinearColor RenderColor;

			bool Hovered = this->CheckHovered(Current, Size);
			bool Clicked = this->LeftMouseClicked && Hovered;

			if (Active)
				RenderColor = this->MainColor;
			else if (Hovered)
				RenderColor = this->FrameHoveredColor;
			else
				RenderColor = this->FrameColor;

			Wrapper::RectFilled(Current, Size, RenderColor);
			Wrapper::Rect(Current, Size, FLinearColor(0.f, 0.f, 0.f, 1.f), 1.f);

			Wrapper::Text(Content, FVector2D(Current.X + (Size.X / 2) - 2, Current.Y + 3), this->TextColor, true, false);

			this->Offset.X += 128;

			return Clicked && !this->TotalOpenPopups;
		}

		bool Button(FString Text)
		{
			this->Offset.Y += 5;

			FVector2D Current = this->Position + this->Offset;
			FVector2D Size = FVector2D(120, 22);
			FLinearColor RenderColor;

			bool Hovered = this->CheckHovered(Current, Size);
			bool Clicked = this->LeftMouseClicked && Hovered;
			bool Down = this->LeftMouseDown && Hovered;

			if (Down)
				RenderColor = this->FramePressedColor;
			else if (Hovered)
				RenderColor = this->FrameHoveredColor;
			else
				RenderColor = this->FrameColor;

			Wrapper::RectFilled(Current, FVector2D(Current.X, Size.Y), RenderColor);
			Wrapper::Rect(Current, FVector2D(Current.X, Size.Y), FLinearColor(0.f, 0.f, 0.f, 1.f), 1.f);

			Wrapper::Text(Text, FVector2D(Current.X + (Size.X / 2) - 2, Current.Y + 3), this->TextColor, true, false);

			this->Offset.Y += 28;

			return Clicked && !this->TotalOpenPopups;
		}

		void Text(FString Content)
		{
			FVector2D Current = this->Position + this->Offset;

			Wrapper::Text(Content, Current, this->TextColor, false, false);

			this->Offset.Y += 16;
		}

		void TextColored(FString Content, FLinearColor Color)
		{
			FVector2D Current = this->Position + this->Offset;

			Wrapper::Text(Content, Current, Color, false, false);

			this->Offset.Y += 16;
		}

		FVector2D GetCurrent()
		{
			return this->Position + this->Offset;
		}

		void Space(double X, double Y)
		{
			this->Offset += FVector2D(X, Y);
		}

		void PushX(double value)
		{
			this->Offset.X = value;
		}

		void PushY(double value)
		{
			this->Offset.Y = value;
		}

		void AddOffset(FVector2D ToAddOffset)
		{
			this->Offset.X += ToAddOffset.X;
			this->Offset.Y += ToAddOffset.Y;
		}

		bool CreateMenu()
		{
			if (!this->AlreadyInitialized)
			{
				this->Initialize();

				this->AlreadyInitialized = true;
			}

			this->TotalOpenPopups = 0;
			this->Offset = FVector2D(0, 0);

			this->Cursor = Variables::PlayerController->GetMousePosition();
			this->LeftMouseClicked = Variables::PlayerController->WasInputKeyJustPressed(Keys::LeftMouseButton);
			this->LeftMouseDown = Variables::PlayerController->IsInputKeyDown(Keys::LeftMouseButton);
			this->RightMouseClicked = Variables::PlayerController->WasInputKeyJustPressed(Keys::RightMouseButton);
			this->RightMouseDown = Variables::PlayerController->IsInputKeyDown(Keys::RightMouseButton);

			static FVector2D StaticCursor;
			static FVector2D StaticCalculated;

			if (this->CheckHovered(FVector2D(this->Position.X, this->Position.Y - 5.0), FVector2D(this->Size.X, 20.0)) && LeftMouseDown)
			{
				if (!StaticCursor)
				{
					StaticCursor = this->Cursor;
				}

				if (!StaticCalculated)
				{
					StaticCalculated = StaticCursor - this->Position;
				}
			}

			if (StaticCursor && StaticCalculated)
			{
				this->Position = this->Cursor - StaticCalculated;
			}

			if (!this->LeftMouseDown)
			{
				StaticCursor = FVector2D();
				StaticCalculated = FVector2D();
			}

			Wrapper::RoundedRectFilled(this->Position, this->Size, 7.f, this->BackgroundColor);
			Wrapper::RoundedRectFilled(FVector2D(this->Position.X + 10, this->Position.Y + 50.0), FVector2D(this->Size.X - 20, this->Size.Y - 60), 7.f, FLinearColor(0.1010f, 0.0892f, 0.0892f, 1.f));

			return true;
		}

		bool RenderMouse()
		{
			Wrapper::RectFilled(FVector2D(this->Cursor.X - 4, this->Cursor.Y - 4), FVector2D(4, 4), this->MainColor, true);
			return true;
		}
	private:
		bool AlreadyInitialized, LeftMouseClicked, RightMouseClicked, LeftMouseDown, RightMouseDown;
		FVector2D Position, Size, Offset, Cursor, ScreenCenter;
		FLinearColor BackgroundColor, MainColor, FrameColor, FrameHoveredColor, FramePressedColor, TextColor;
		int TotalOpenPopups; double CurrentAlpha = 0.0;
	};

	void DrawMenu()
	{
		if (Wrapper::IsSafeToRender())
		{
			static GUI Framework;
			static int MenuTab = int(0);

			Framework.CreateMenu();

			Framework.PushX(10);
			Framework.PushY(17);

			if (Framework.MenuButton(FString(L"Aimbot"), MenuTab == 0)) MenuTab = 0;
			if (Framework.MenuButton(FString(L"Visuals"), MenuTab == 1)) MenuTab = 1;
			if (Framework.MenuButton(FString(L"World"), MenuTab == 2)) MenuTab = 2;
			if (Framework.MenuButton(FString(L"Misc"), MenuTab == 3)) MenuTab = 3;
			if (Framework.MenuButton(FString(L"Exploits"), MenuTab == 4)) MenuTab = 4;

			Framework.PushX(30);
			Framework.PushY(50);

			if (MenuTab == 0)
			{
				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Aimbot"));

				Framework.Space(15, 0);

				Framework.Checkbox(FString(L"Enable"), &Settings::Aimbot::Aimbot);
				Framework.Checkbox(FString(L"Target line"), &Settings::Aimbot::TargetLine);
				Framework.Checkbox(FString(L"Draw FOV"), &Settings::Aimbot::DrawFOV);
				Framework.Slider(FString(L"FOV"), &Settings::Aimbot::FOV, 0.00, 80.0);
				Framework.Space(0, 5);

				if (Settings::Aimbot::SilentAimbot)
				{

				}
				else if (Settings::Aimbot::StickySilentAim)
				{
					Framework.Checkbox(FString(L"Aim Prediction"), &Settings::Aimbot::PredictProjectiles);
					Framework.Checkbox(FString(L"Sticky Camera"), &Settings::Aimbot::StickySilent::StickyCameraSpin);
				}
				else
				{
					Framework.Checkbox(FString(L"Aim Prediction"), &Settings::Aimbot::PredictProjectiles);

					static bool SmoothingActive = false;
					FString SmoothingTypes[] = { FString(L"Disabled"), FString(L"Berziers Curve"), FString(L"Interpolate"), FString(L"Constant") };
					Framework.Combobox(FString(L"Smoothing"), &SmoothingActive, &Settings::Aimbot::SmoothingType, SmoothingTypes, sizeof(SmoothingTypes) / sizeof(FString));
					Framework.Slider(FString(L"Smoothing (%)"), &Settings::Aimbot::Smoothing, 0.0, 10.0);
					Framework.Slider(FString(L"Hit Chance (%)"), &Settings::Aimbot::ShakeSpeed, 0.0, 10.0);
				}

				Framework.Space(0, 5);

				static bool BoneTypeActive = false;
				FString BoneTypes[] = { FString(L"Head"), FString(L"Neck"), FString(L"Chest"), FString(L"Random") };
				Framework.Combobox(FString(L"Bone"), &BoneTypeActive, &Settings::Aimbot::BoneType, BoneTypes, sizeof(BoneTypes) / sizeof(FString));

				static bool KeyTypeActive = false;
				FString KeyTypes[] = { FString(L"RMB"), FString(L"LMB"), FString(L"Shift") };
				Framework.Combobox(FString(L"Keybind"), &KeyTypeActive, &Settings::Aimbot::KeyType, KeyTypes, sizeof(KeyTypes) / sizeof(FString));

				Framework.Slider(FString(L"Distance Limit (m)"), &Settings::Aimbot::MaxDistance, 0.0, 350.0);

				Framework.PushY(50);
				Framework.PushX(420);

				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Misc"));

				Framework.Space(15, 0);
				Framework.Checkbox(FString(L"Trigger bot"), &Settings::Aimbot::Triggerbot);
				Framework.Checkbox(FString(L"Shotgun Only"), &Settings::Aimbot::OnlyShotgun);
				Framework.Checkbox(FString(L"Visible Only"), &Settings::Aimbot::VisableOnly);
				Framework.Checkbox(FString(L"Skip Knocked"), &Settings::Aimbot::SkipKnocked);

				Framework.Space(0, 10);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Color"));

				Framework.Space(15, 0);

				static bool TargetLineActive = false;
				Framework.ColorPicker(L"Target Line", &Settings::Colors::TargetLine, &TargetLineActive);

				static bool FieldOfViewActive = false;
				Framework.ColorPicker(L"FOV", &Settings::Colors::FieldOfView, &FieldOfViewActive);
			}
			else if (MenuTab == 1)
			{
				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Visuals"));

				Framework.Space(15, 0);
				static bool BoxTypeActive = false;
				FString BoxTypes[] = { FString(L"Disabled"), FString(L"2D"), FString(L"Corner") };
				Framework.Combobox(FString(L"Box"), &BoxTypeActive, &Settings::Player::BoxType, BoxTypes, sizeof(BoxTypes) / sizeof(FString));
				static bool LineTypeActive = false;
				FString linetypes[] = { FString(L"Disabled"), FString(L"Bottom"), FString(L"Middle"), FString(L"Top") };
				Framework.Combobox(FString(L"Lines"), &LineTypeActive, &Settings::Player::LineType, linetypes, sizeof(linetypes) / sizeof(FString));
				Framework.Checkbox(FString(L"Skeleton"), &Settings::Player::Skeleton);
				Framework.Checkbox(FString(L"Distance"), &Settings::Player::Distance);
				Framework.Checkbox(FString(L"Season Rank"), &Settings::Player::Rank);
				Framework.Checkbox(FString(L"Username + Level"), &Settings::Player::Name);
				Framework.Checkbox(FString(L"Platform"), &Settings::Player::Platform);
				Framework.Checkbox(FString(L"Weapon"), &Settings::Player::Weapon);

				Framework.Slider(FString(L"Distance Limit (m)"), &Settings::Player::MaxDistance, 0.0, 500.0);

				Framework.PushY(50);
				Framework.PushX(420);

				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Radar"));

				Framework.Space(15, 0);
				Framework.Checkbox(FString(L"Enable Radar"), &Settings::Player::Radar);
				Framework.Checkbox(FString(L"Minimap Mode"), &Settings::Player::Minimap);
				Framework.Slider(FString(L"(X%)"), &Settings::Player::RadarX, 20.0, 1600.0);
				Framework.Slider(FString(L"(Y%)"), &Settings::Player::RadarY, 20.0, 770.0);

				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Color"));

				Framework.Space(15, 0);
				static bool TeammateColorActive = false;
				Framework.ColorPicker(FString(L"Teammate"), &Settings::Colors::TeammateColor, &TeammateColorActive);

				static bool BoxVisibleActive = false;
				Framework.ColorPicker(FString(L"Box Visible"), &Settings::Colors::BoxVisible, &BoxVisibleActive);

				static bool BoxInVisibleActive = false;
				Framework.ColorPicker(FString(L"Box Invisible"), &Settings::Colors::BoxInVisible, &BoxInVisibleActive);

				static bool SkeletonVisibleActive = false;
				Framework.ColorPicker(FString(L"Skeleton Visible"), &Settings::Colors::SkeletonVisible, &SkeletonVisibleActive);

				static bool SkeletonInVisibleActive = false;
				Framework.ColorPicker(FString(L"Skeleton Invisible"), &Settings::Colors::SkeletonInVisible, &SkeletonInVisibleActive);

				static bool SnaplineVisibleActive = false;
				Framework.ColorPicker(FString(L"Snapline Visible"), &Settings::Colors::SnaplineVisible, &SnaplineVisibleActive);

				static bool SnaplineInVisibleActive = false;
				Framework.ColorPicker(FString(L"Snapline Invisible"), &Settings::Colors::SnaplineInVisible, &SnaplineInVisibleActive);
			}
			else if (MenuTab == 2)
			{
				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"World"));

				Framework.Space(15, 0);
				Framework.Checkbox(FString(L"Loot ESP"), &Settings::Environment::Pickup);
				Framework.Checkbox(FString(L"Vehicle ESP"), &Settings::Environment::Vehicle);
				Framework.Checkbox(FString(L"SupplyDrop/Llama ESP"), &Settings::Environment::SupplyDrop);
				Framework.Checkbox(FString(L"Drone/Chest/AmmoBox ESP"), &Settings::Environment::Container);
				Framework.Checkbox(FString(L"Storm Prediction ESP"), &Settings::Misc::StormPrediction);
				Framework.Checkbox(FString(L"WeakPoint ESP"), &Settings::Environment::Weakspot);
				Framework.Checkbox(FString(L"WeakPoint Aim"), &Settings::Environment::WeakspotAim);
				Framework.Slider(FString(L"Distance Limit (m)"), &Settings::Environment::MaxDistance, 0.0, 500.0);

				Framework.PushY(50);
				Framework.PushX(420);

				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Misc"));

				Framework.Space(15, 0);
				Framework.Checkbox(FString(L"Performance mode"), &Settings::Environment::DisableRMU);
				Framework.Text(FString(L"- Boosts FPS"));
				Framework.Space(0, 5);

				Framework.Checkbox(FString(L"Chest/AmmoBox Chams"), &Settings::Environment::Chams);
			}
			else if (MenuTab == 3)
			{
				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Misc"));

				Framework.Space(15, 0);
				static bool CrosshairModesActive = false;
				FString CrosshairModes[] = { FString(L"Disabled"), FString(L"Standard"), FString(L"Standard2"), FString(L"Dot"),FString(L"Warrior") };
				Framework.Combobox(L"Crosshair", &CrosshairModesActive, &Settings::Misc::CrosshairMode, CrosshairModes, sizeof(CrosshairModes) / sizeof(FString));
				Framework.Space(0, 5);

				Framework.Checkbox(FString(L"Outline Text"), &Settings::Misc::TextOutline);

				Framework.Text(L"");

				Framework.Text(L"UserName: ");

				Framework.AddOffset(FVector2D(100.0, -16.0));

				FVector2D Current = Framework.GetCurrent();

				Wrapper::Text(UKismetSystemLibrary::GetPlatformUserName(), Current, FLinearColor(1.f, 0.5f, 0.f, 1.f), false, false, false, false);

				Framework.AddOffset(FVector2D(-100.0, 16.0));

				Framework.Text(FString(L"Version: "));

				Framework.AddOffset(FVector2D(100.0, -16.0));

				Current = Framework.GetCurrent();

				Wrapper::Text(UKismetSystemLibrary::GetEngineVersion(), Current, FLinearColor(1.f, 0.5f, 0.f, 1.f), false, false, false, false);

				Framework.AddOffset(FVector2D(-100.0, 16.0));
			}
			else if (MenuTab == 4)
			{
				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Exploits"));

				Framework.Space(15, 0);
				Framework.Checkbox(FString(L"Enable Exploits"), &Settings::Exploits::Hook);
				Framework.Text(FString(L"- For legit players, don't enable exploits!"));

				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Weapon"));

				Framework.Space(15, 0);
				Framework.Checkbox(FString(L"Silent Aimbot"), &Settings::Aimbot::SilentAimbot);
				Framework.Checkbox(FString(L"Sticky Silent"), &Settings::Aimbot::StickySilentAim);
				Framework.Checkbox(FString(L"Bullet Teleport"), &Settings::Exploits::BulletTeleport);
				Framework.Checkbox(FString(L"Magic Bullet"), &Settings::Exploits::MagicWeapon);
				Framework.Checkbox(FString(L"No Recoil"), &Settings::Exploits::NoRecoil);
				Framework.Checkbox(FString(L"No Spread"), &Settings::Exploits::NoSpread);

				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Myself"));

				Framework.Space(15, 0);
				Framework.Checkbox(FString(L"Camera FOV Changer"), &Settings::Exploits::CameraFOVChanger);
				Framework.Slider(FString(L"FOV"), &Settings::Exploits::CameraFOV, 65.0, 120.0);

				Framework.Checkbox(FString(L"Player Spinbot"), &Settings::Exploits::Spinbot);
				Framework.Slider(FString(L"Spin Speed (%)"), &Settings::Exploits::SpinSpeed, 1.0, 10.0);

				Framework.Checkbox(FString(L"Player Fly"), &Settings::Exploits::PlayerFly);

				Framework.PushY(50);
				Framework.PushX(420);

				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Player Chams"));

				Framework.Space(15, 0);
				Framework.Checkbox(FString(L"Player Chams"), &Settings::Exploits::Chams);
				Framework.Checkbox(FString(L"Wireframe Chams"), &Settings::Exploits::Wireframe);
				Framework.Checkbox(FString(L"Cham through walls"), &Settings::Exploits::Glow);

				static bool ChamSkinColorActive = false;
				Framework.ColorPicker(FString(L"Chams color"), &Settings::Colors::SkinColor, &ChamSkinColorActive);

				Framework.Space(0, 5);
				Framework.Space(-15, 0);
				Framework.Text(FString(L"Vehicle"));

				Framework.Space(15, 0);
				Framework.Checkbox(FString(L"Infinite Fuel"), &Settings::Exploits::InfiniteFuel);
				Framework.Checkbox(FString(L"Vehicle Flight"), &Settings::Exploits::CarFly);
				Framework.Slider(FString(L"Speed"), &Settings::Exploits::CarSpeed, 1.0, 30.0);

				Framework.Space(15, 0);
			}

			Framework.RenderMouse();
		}
	}
}