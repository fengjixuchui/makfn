#pragma once

namespace Render
{
	void Line(FVector2D A, FVector2D B, FLinearColor Color, float Thickness)
	{
		Variables::Canvas->K2_DrawLine(A, B, Thickness, Color);
	}


	void Text(UFont* Font, FString Content, FVector2D Screen, FLinearColor Color, bool CenterX, bool CenterY, bool Outlined , double Scale = 1.00)
	{
		Variables::Canvas->K2_DrawText(Font, Content, Screen, FVector2D(Scale, Scale), Color, Scale, CenterX, CenterY, Outlined);
	}

	void Diamond(FVector2D ScreenPosition, FVector2D Radius, FLinearColor RenderColor)
	{
		FVector2D DiamondPoints[4];
		DiamondPoints[0] = { ScreenPosition.X, ScreenPosition.Y - Radius.Y };
		DiamondPoints[1] = { ScreenPosition.X + Radius.X, ScreenPosition.Y };
		DiamondPoints[2] = { ScreenPosition.X, ScreenPosition.Y + Radius.Y };
		DiamondPoints[3] = { ScreenPosition.X - Radius.X, ScreenPosition.Y };

		Variables::Canvas->K2_DrawPolygon(nullptr, DiamondPoints[0], Radius, 4, RenderColor);
	}

	void DrawRectFilled(FVector2D Position, FVector2D Size, FLinearColor RenderColor)
	{
		for (int i = 0; i < Size.Y; i++)
		{
			Render::Line(FVector2D(Position.X, Position.Y + i), FVector2D(Position.X + Size.X, Position.Y + i), RenderColor, 1.f);
		}
	}

	void DrawRectRoundedFilled(FVector2D Position, FVector2D Size, FLinearColor RenderColor, float Rounding)
	{
		Render::DrawRectFilled(FVector2D{ Position.X + Rounding, Position.Y }, FVector2D(Size.X - (Rounding * 2), Size.Y), RenderColor);
		Render::DrawRectFilled(FVector2D{ Position.X, Position.Y + Rounding }, FVector2D(Size.X, Size.Y - (Rounding * 2)), RenderColor);

		FVector2D diamondRadius = { Rounding - 1, Rounding - 2 };
		Render::Diamond(FVector2D{ Position.X + Rounding - 2, Position.Y + Rounding + 3 }, diamondRadius, RenderColor);
		Render::Diamond(FVector2D{ Position.X + Size.X - Rounding + 1, Position.Y + Rounding + 3 }, diamondRadius, RenderColor);
		Render::Diamond(FVector2D{ Position.X + Rounding - 2, Position.Y + Size.Y - Rounding + 5 }, diamondRadius, RenderColor);
		Render::Diamond(FVector2D{ Position.X + Size.X - Rounding + 1, Position.Y + Size.Y - Rounding + 5 }, diamondRadius, RenderColor);
	}

	void DrawRect(FVector2D Position, FVector2D Size, FLinearColor RenderColor, float Thickness)
	{
		Render::Line(FVector2D(Position.X, Position.Y), FVector2D(Position.X + Size.X, Position.Y), RenderColor, Thickness);
		Render::Line(FVector2D(Position.X + Size.X, Position.Y), FVector2D(Position.X + Size.X, Position.Y + Size.Y), RenderColor, Thickness);
		Render::Line(FVector2D(Position.X + Size.X, Position.Y + Size.Y), FVector2D(Position.X, Position.Y + Size.Y), RenderColor, Thickness);
		Render::Line(FVector2D(Position.X, Position.Y + Size.Y), FVector2D(Position.X, Position.Y), RenderColor, Thickness);
	}

	float Interpolate(FVector2D Start, FVector2D End, float X)
	{
		float slope = (End.Y - Start.Y) / (End.X - Start.X);
		float Y = Start.Y + slope * (X - Start.X);
		return Y;
	}

	void Triangle(FVector2D Position, FVector2D Size, FLinearColor RenderColor, float Thickness, bool Filled)
	{
		FVector2D V1(Position.X, Position.Y - (2.0f / 3.0f) * Size.Y);
		FVector2D V2(Position.X - Size.X / 2.0f, Position.Y + (1.0f / 3.0f) * Size.Y);
		FVector2D V3(Position.X + Size.X / 2.0f, Position.Y + (1.0f / 3.0f) * Size.Y);

		if (Filled)
		{
			for (float X = V2.X; X <= V3.X; X += 1.0f)
			{
				float YStart, YEnd;

				if (X <= V1.X)
				{
					YStart = Interpolate(V2, V1, X);
					YEnd = Interpolate(V2, V3, X);
				}
				else
				{
					YStart = Interpolate(V3, V1, X);
					YEnd = Interpolate(V2, V3, X);
				}

				Render::Line(FVector2D(X, YStart), FVector2D(X, YEnd), RenderColor, Thickness);
			}
		}
		else
		{
			Render::Line(V1, V2, RenderColor, Thickness);
			Render::Line(V2, V3, RenderColor, Thickness);
			Render::Line(V3, V1, RenderColor, Thickness);
		}
	}

	void Circle(FVector2D Position, FLinearColor color, double radius, double segments, bool filled)
	{
		double step = M_PI * 2.0 / segments;
		int count = 0;
		FVector2D vectors[128];

		for (double deg = 0; deg < M_PI * 2; deg += step)
		{
			double x_1 = radius * UKismetMathLibrary::cos(deg) + Position.X;
			double y_1 = radius * UKismetMathLibrary::sin(deg) + Position.Y;
			double x_2 = radius * UKismetMathLibrary::cos(deg + step) + Position.X;
			double y_2 = radius * UKismetMathLibrary::sin(deg + step) + Position.Y;

			vectors[count].X = x_1;
			vectors[count].Y = y_1;
			vectors[count + 1].X = x_2;
			vectors[count + 1].Y = y_2;

			if (filled)
			{
				for (double X = x_1; X <= x_2; X += 1.0)
				{
					double Y = Position.Y + radius * UKismetMathLibrary::sin(UKismetMathLibrary::acos((X - Position.X) / radius));
					Render::Line(FVector2D(X, y_1), FVector2D(X, Y), color, 1.0f);
					Render::Line(FVector2D(X, Y), FVector2D(X, y_2), color, 1.0f);
				}
			}
			else
			{
				Render::Line(FVector2D(vectors[count].X, vectors[count].Y), FVector2D(x_2, y_2), color, 1.0f);
			}
		}
	}
}

namespace Wrapper
{
	struct DrawListMember
	{
		int DrawType;

		FString Content;

		FVector2D PositionA, PositionB;
		FLinearColor Color;
		UFont* Font;

		float Thickness;

		double Radius;
		double Segments;
		bool Filled;

		bool Outlined;
		bool Centered;

		bool Rounded;
	};

	int LastForegroundIndex = 0;
	DrawListMember ForegroundDrawList[9999];

	int LastDefaultIndex = 0;
	DrawListMember DefaultDrawList[9999];

	void ClearTargets()
	{
		LastDefaultIndex = 0;
		LastForegroundIndex = 0;
	}

	bool IsSafeToRender()
	{
		bool ReturnValue = UKismetSystemLibrary::IsValid(Variables::Canvas) && MediumFont;
		if (!ReturnValue)
		{
			ClearTargets();
		}

		return ReturnValue;
	}

	void Render()
	{
		for (int i = 0; i < LastDefaultIndex; i++)
		{
			DrawListMember Member = DefaultDrawList[i];

			if (Member.DrawType == 1)
			{
				Render::Line(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 2)
			{
				Render::DrawRect(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 3)
			{
				Render::DrawRectFilled(Member.PositionA, Member.PositionB, Member.Color);
			}
			else if (Member.DrawType == 4)
			{
				Render::Text(Member.Font, Member.Content, Member.PositionA, Member.Color, Member.Centered, false, Member.Outlined);
			}
			else if (Member.DrawType == 5)
			{
				Render::Circle(Member.PositionA, Member.Color, Member.Radius, Member.Segments, Member.Filled);
			}
			else if (Member.DrawType == 6)
			{
				Render::DrawRectRoundedFilled(Member.PositionA, Member.PositionB, Member.Color, Member.Segments);
			}
			else if (Member.DrawType == 7)
			{
				Render::Triangle(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness, Member.Filled);
			}
		}

		for (int i = 0; i < LastForegroundIndex; i++)
		{
			DrawListMember Member = ForegroundDrawList[i];

			if (Member.DrawType == 1)
			{
				Render::Line(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 2)
			{
				Render::DrawRect(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness);
			}
			else if (Member.DrawType == 3)
			{
				Render::DrawRectFilled(Member.PositionA, Member.PositionB, Member.Color);
			}
			else if (Member.DrawType == 4)
			{
				Render::Text(Member.Font, Member.Content, Member.PositionA, Member.Color, Member.Centered, false, Member.Outlined);
			}
			else if (Member.DrawType == 5)
			{
				Render::Circle(Member.PositionA, Member.Color, Member.Radius, Member.Segments, Member.Filled);
			}
			else if (Member.DrawType == 6)
			{
				Render::DrawRectRoundedFilled(Member.PositionA, Member.PositionB, Member.Color, Member.Segments);
			}
			else if (Member.DrawType == 7)
			{
				Render::Triangle(Member.PositionA, Member.PositionB, Member.Color, Member.Thickness, Member.Filled);
			}
		}

		Wrapper::ClearTargets();
	}

	void Line(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, float Thickness, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 1;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;
		Member.Thickness = Thickness;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Rect(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, float Thickness, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 2;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;
		Member.Thickness = Thickness;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void RectFilled(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 3;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void RoundedRectFilled(FVector2D PositionA, FVector2D PositionB, float Segments, FLinearColor RenderColor, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 6;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Segments = Segments;
		Member.Color = RenderColor;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Text(FString Content, FVector2D Position, FLinearColor RenderColor, bool Centered = false, bool Outlined = Settings::Misc::TextOutline, bool ForceForeground = false, UFont* Font = MediumFont)
	{
		DrawListMember Member;

		Member.DrawType = 4;
		Member.Content = FString(Content.c_str(), Content.Size());
		Member.PositionA = Position;
		Member.Color = RenderColor;
		Member.Centered = Centered;
		Member.Outlined = Outlined;
		Member.Font = Font;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Circle(FVector2D Position, FLinearColor RenderColor, double Radius, double Segments, bool Filled = false, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 5;
		Member.PositionA = Position;
		Member.Color = RenderColor;
		Member.Radius = Radius;
		Member.Segments = Segments;
		Member.Filled = Filled;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}

	void Triangle(FVector2D PositionA, FVector2D PositionB, FLinearColor RenderColor, bool Filled = false, bool ForceForeground = false)
	{
		DrawListMember Member;

		Member.DrawType = 7;
		Member.PositionA = PositionA;
		Member.PositionB = PositionB;
		Member.Color = RenderColor;
		Member.Filled = Filled;

		if (ForceForeground)
		{
			ForegroundDrawList[LastForegroundIndex] = Member;
			LastForegroundIndex += 1;
		}
		else
		{
			DefaultDrawList[LastDefaultIndex] = Member;
			LastDefaultIndex += 1;
		}
	}
}

namespace Radar
{
	void Range(double* X, double* Y, float range)
	{
		if (UKismetMathLibrary::abs((*X)) > range || UKismetMathLibrary::abs((*Y)) > range)
		{
			if ((*Y) > (*X))
			{
				if ((*Y) > -(*X))
				{
					(*X) = range * (*X) / (*Y);
					(*Y) = range;
				}
				else
				{
					(*Y) = -range * (*Y) / (*X);
					(*X) = -range;
				}
			}
			else
			{
				if ((*Y) > -(*X))
				{
					(*Y) = range * (*Y) / (*X);
					(*X) = range;
				}
				else
				{
					(*X) = -range * (*X) / (*Y);
					(*Y) = -range;
				}
			}
		}
	}

	FVector2D RotatePoint(FVector2D radar_pos, FVector2D radar_size, FVector LocalLocation, FVector TargetLocation)
	{
		auto dx = TargetLocation.X - LocalLocation.X;
		auto dy = TargetLocation.Y - LocalLocation.Y;

		auto X = dy * -1;
		X *= -1;
		auto Y = dx * -1;

		double calcualted_range = 34 * 1000;

		Radar::Range(&X, &Y, calcualted_range);

		int rad_x = (int)radar_pos.X;
		int rad_y = (int)radar_pos.Y;

		double r_siz_x = radar_size.X;
		double r_siz_y = radar_size.Y;

		int x_max = (int)r_siz_x + rad_x - 5;
		int y_max = (int)r_siz_y + rad_y - 5;

		auto return_value = FVector2D();

		return_value.X = rad_x + ((int)r_siz_x / 2 + int(X / calcualted_range * r_siz_x));
		return_value.Y = rad_y + ((int)r_siz_y / 2 + int(Y / calcualted_range * r_siz_y));

		if (return_value.X > x_max)
			return_value.X = x_max;

		if (return_value.X < rad_x)
			return_value.X = rad_x;

		if (return_value.Y > y_max)
			return_value.Y = y_max;

		if (return_value.Y < rad_y)
			return_value.Y = rad_y;

		return return_value;
	}

	void Add(FVector WorldLocation, FVector LocalLocation, FLinearColor Color, FVector2D RadarPos, FVector2D RadarSize, double Size)
	{
		FVector2D Projected = Radar::RotatePoint(RadarPos, RadarSize, LocalLocation, WorldLocation);

		Wrapper::Triangle(Projected, FVector2D(15.0, 15.0), Color, false, false);

		if (Custom::InArea(Projected.X, Projected.Y, RadarPos.X + 20, RadarPos.Y + 20, RadarSize.X - (20 * 2), RadarSize.Y - (20 * 2)))
		{
			Wrapper::Line(Projected, FVector2D(RadarPos.X + RadarSize.X / 2, RadarPos.Y + RadarSize.Y / 2), Color, 1.f);
		}
	}
}

namespace Custom
{
	FMatrix RotatorToMatrix(FRotator Rotation)
	{
		FMatrix Copy = {};

		double Pitch = (Rotation.Pitch * M_PI / 180.0);
		double Yaw = (Rotation.Yaw * M_PI / 180.0);
		double Roll = (Rotation.Roll * M_PI / 180.0);

		double SP = UKismetMathLibrary::sin(Pitch);
		double CP = UKismetMathLibrary::cos(Pitch);
		double SY = UKismetMathLibrary::sin(Yaw);
		double CY = UKismetMathLibrary::cos(Yaw);
		double SR = UKismetMathLibrary::sin(Roll);
		double CR = UKismetMathLibrary::cos(Roll);

		Copy.XPlane.X = CP * CY;
		Copy.XPlane.Y = CP * SY;
		Copy.XPlane.Z = SP;

		Copy.YPlane.X = SR * SP * CY - CR * SY;
		Copy.YPlane.Y = SR * SP * SY + CR * CY;
		Copy.YPlane.Z = -SR * CP;

		Copy.ZPlane.X = -(CR * SP * CY + SR * SY);
		Copy.ZPlane.Y = CY * SR - CR * SP * SY;
		Copy.ZPlane.Z = CR * CP;

		Copy.WPlane.W = 1.0;

		return Copy;
	}

	void WorldToScreenRaw(FVector WorldLocation, FVector2D* ScreenLocation)
	{
		FMatrix Matrix = RotatorToMatrix(Variables::CameraRotation);

		FVector XAxis = FVector(Matrix.XPlane.X, Matrix.XPlane.Y, Matrix.XPlane.Z);
		FVector YAxis = FVector(Matrix.YPlane.X, Matrix.YPlane.Y, Matrix.YPlane.Z);
		FVector ZAxis = FVector(Matrix.ZPlane.X, Matrix.ZPlane.Y, Matrix.ZPlane.Z);

		FVector DeltaCoordinates = WorldLocation - Variables::CameraLocation;

		FVector Transform = FVector(DeltaCoordinates.Dot(YAxis), DeltaCoordinates.Dot(ZAxis), DeltaCoordinates.Dot(XAxis));
		if (Transform.Z < 0.01)
		{
			Transform.Z = 0.01;
		}

		ScreenLocation->X = Variables::ScreenCenter.X + Transform.X * (Variables::ScreenCenter.X / UKismetMathLibrary::tan(Variables::FieldOfView * M_PI / 360.0)) / Transform.Z;
		ScreenLocation->Y = Variables::ScreenCenter.Y - Transform.Y * (Variables::ScreenCenter.X / UKismetMathLibrary::tan(Variables::FieldOfView * M_PI / 360.0)) / Transform.Z;
	}

	FVector2D CalculateControlPoint(const FVector2D& Start, const FVector2D& End, double Offset)
	{
		FVector2D MidPoint = (Start + End) / 2.0;
		FVector2D Direction = End - Start;
		FVector2D Perpendicular(-Direction.Y, Direction.X);
		Perpendicular = Perpendicular / UKismetMathLibrary::sqrt(Perpendicular.X * Perpendicular.X + Perpendicular.Y * Perpendicular.Y); // Normalize
		FVector2D ControlPoint = MidPoint + Perpendicular * Offset;
		return ControlPoint;
	}

	void BezierCurveLine(FVector2D Start, FVector2D End, FLinearColor Color, float Thickness, float Size, bool RightSide = false) 
	{
		const int NumSegments = 20;

		FVector2D PreviousPoint = Start;

		FVector2D ControlPoint = CalculateControlPoint(Start, End, double(Size));
		if (RightSide) {
			FVector2D MidPoint = (Start + End) / 2.0;
			FVector2D MidToControl = ControlPoint - MidPoint;
			FVector2D MirroredMidToControl(MidToControl.X, -MidToControl.Y);
			ControlPoint = MidPoint + MirroredMidToControl;
		}

		for (int i = 1; i <= NumSegments; ++i) 
		{
			float t = i / float(NumSegments);

			double CurrentPointX = (1 - t) * (1 - t) * Start.X + 2 * (1 - t) * t * ControlPoint.X + t * t * End.X;
			double CurrentPointY = (1 - t) * (1 - t) * Start.Y + 2 * (1 - t) * t * ControlPoint.Y + t * t * End.Y;

			Wrapper::Line(PreviousPoint, FVector2D(CurrentPointX, CurrentPointY), Color, Thickness);

			PreviousPoint = FVector2D(CurrentPointX, CurrentPointY);
		}
	}
}