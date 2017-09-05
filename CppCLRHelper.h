#pragma once

namespace Ambiesoft {
	System::Windows::Forms::DialogResult WarningMessageBox(System::String^ text);
	System::Windows::Forms::DialogResult WarningMessageBox(System::Exception^ ex);


}