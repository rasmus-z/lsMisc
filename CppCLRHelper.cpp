#include "stdafx.h"
#include "CppCLRHelper.h"

namespace Ambiesoft {
	using namespace System;
	using namespace System::Windows::Forms;

	DialogResult WarningMessageBox(String^ text)
	{
		return MessageBox::Show(
			text,
			Application::ProductName,
			MessageBoxButtons::OK,
			MessageBoxIcon::Warning);
	}
	DialogResult WarningMessageBox(Exception^ ex)
	{
		return WarningMessageBox(ex->Message);
	}


}