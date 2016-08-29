using System;
using System.Runtime.InteropServices;
using System.Text;

namespace LoginServer
{
    
    ///사용법
    ///using INI;
    ///Create a INIFile like this
    ///Hide Copy Code
    ///INIFile ini = new INIFile("test.ini");
    ///Use IniWriteValue to write a new value to a specific key in a section or ,
    ///Use IniReadValue to read a value FROM a key in a specific Section.

    /// Create a New INI file to store or load data
    public class IniFile
    {
        public string path;

        [DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section,
            string key, string val, string filePath);
        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section,
                    string key, string def, StringBuilder retVal,
            int size, string filePath);

        /// INIFile Constructor.
        public IniFile(string INIPath)
        {
            path = INIPath;
        }
        /// Write Data to the INI File
        public void IniWriteValue(string Section, string Key, string Value)
        {
            WritePrivateProfileString(Section, Key, Value, this.path);
        }

        /// Read Data Value From the Ini File
        public string IniReadValue(string Section, string Key)
        {
            StringBuilder temp = new StringBuilder(255);
            int i = GetPrivateProfileString(Section, Key, "", temp,
                                            255, this.path);
            return temp.ToString();

        }
    }
}
