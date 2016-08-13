using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Microsoft.Owin.Hosting;
using System.Web.Http;

namespace LoginServer
{
    public class Program
    {
        static void Main()
        {
            StartOptions options = new StartOptions();
            options.Urls.Add("http://*:8258");

            // Start OWIN host 
            using (WebApp.Start<Startup>(options))
            {
                Console.WriteLine("LoginServer Running.. : " + Environment.MachineName);

                //이 using 안에서 빠져나가면 서버 종료됨..
                Console.ReadLine();
            }

        }
    }
}
