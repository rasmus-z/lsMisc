#include "stdafx.h"

#include "clrHelper.h"

#ifdef __cplusplus_cli

using namespace System;
using namespace System::Windows::Forms;
using namespace std;

namespace Ambiesoft {


	DialogResult Alert(String^ text)
	{
		return MessageBox::Show(
			text,
			Application::ProductName,
			MessageBoxButtons::OK,
			MessageBoxIcon::Warning);
	}
	DialogResult Alert(Exception^ ex)
	{
		return Alert(ex->Message);
	}


}

#endif  // __cplusplus_cli
