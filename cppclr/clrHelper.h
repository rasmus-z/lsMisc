#pragma once


#ifdef __cplusplus_cli

namespace Ambiesoft {

	ref class COpenCommandGetResult
	{
	internal:
		static System::Text::StringBuilder^ sbOut;
		static System::Text::StringBuilder^ sbErr;
		static void OnOutputDataReceived(System::Object ^sender, System::Diagnostics::DataReceivedEventArgs ^e)
		{
			sbOut->Append(e->Data);
		}
		static void OnErrorDataReceived(System::Object ^sender, System::Diagnostics::DataReceivedEventArgs ^e)
		{
			sbErr->Append(e->Data);
		}

		static void OpenCommnadGetResultImpl(System::String^ fileName, System::String^ arguments,
			System::Text::Encoding^ encoding,
			System::String^% out,
			System::String^% err)
		{
			System::Diagnostics::Process process;

			System::Diagnostics::ProcessStartInfo startInfo;
			startInfo.FileName = fileName;
			startInfo.Arguments = arguments;

			startInfo.CreateNoWindow = true;
			startInfo.UseShellExecute = false;

			startInfo.StandardOutputEncoding = encoding;
			startInfo.RedirectStandardOutput = true;

			startInfo.StandardErrorEncoding = encoding;
			startInfo.RedirectStandardError = true;

			process.OutputDataReceived += gcnew System::Diagnostics::DataReceivedEventHandler(&COpenCommandGetResult::OnOutputDataReceived);
			process.ErrorDataReceived += gcnew System::Diagnostics::DataReceivedEventHandler(&COpenCommandGetResult::OnErrorDataReceived);

			process.StartInfo = %startInfo;
			sbOut = gcnew System::Text::StringBuilder();
			sbErr = gcnew System::Text::StringBuilder();

			process.Start();

			process.BeginOutputReadLine();
			process.BeginErrorReadLine();

			process.WaitForExit();

			out = sbOut->ToString();
			err = sbErr->ToString();

		}
	};
	static void OpenCommnadGetResult(System::String^ fileName, System::String^ arguments,
		System::Text::Encoding^ encoding,
		System::String^% out,
		System::String^% err)
	{
		COpenCommandGetResult::OpenCommnadGetResultImpl(
			fileName, arguments,
			encoding,
			out, err);
	}



	System::Windows::Forms::DialogResult Alert(System::String^ text);
	System::Windows::Forms::DialogResult Alert(System::Exception^ ex);


}


#endif