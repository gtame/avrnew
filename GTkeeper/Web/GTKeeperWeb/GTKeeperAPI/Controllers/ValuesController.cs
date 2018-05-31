using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using GTKeeperAPI.Models;

namespace GTKeeperAPI.Controllers
{
    [Route("api/[controller]")]
    public class ValuesController : Controller
    {
        private Devices devices;

        public ValuesController()
        {
            devices= new Devices();
            devices.Add
            (
                new Device()
                {
                    Imei = "123456789012345",
                    Nombre = "Test1",
                    AvisosSMS = SmsAviso.SMSNone,
                    MovilAviso = "653316799",
                    MotorDiesel = false,
                    NumAbono=1,
                    NumPuertos=15

                }
                
            )
        }

        // GET api/values
        [HttpGet]
        public IEnumerable<string> Get()
        {
            return new string[] { "value1", "value2" };
        }

        // GET api/values/5
        [HttpGet("{id}")]
        public string Get(int id)
        {
            return "value";
        } 


        [HttpPost("programacion")]
        public  IActionResult Programacion(string imei,string lup,string luc,IFormFile submitted)
        {
            //long size = files.Sum(f => f.Length);

            // full path to file in temp location
            var filePath = Path.GetTempFileName();

         
                if (submitted.Length > 0)
                {
                    using (var stream = new FileStream(filePath, FileMode.Create))
                    {
                    submitted.CopyTo(stream);
                    }
                }
            

            string[] lines = System.IO.File.ReadAllLines(filePath);
            foreach(string line in lines)
            {
                System.Diagnostics.Debug.WriteLine(line);

            }

            // process uploaded files
            // Don't rely on or trust the FileName property without validation.

            //Todo recibido OK
            //return Ok("+R:O\r\n");
            //REcibido Error! :(
            //return Ok("+R:E\r\n");

            //Recibido y pasamos configuracion
            //return Ok("+R:O\r\n+C:12765331679911111215\r\n");



            //Recibido y pasamos programacion
            //return Ok("+R:O\r\n+C:12765331679911111215\r\n+D:O\r\n");

            
            //03101010120001200000
            //03->Sector
            //127-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
            //1200 -> Ejecucion * Hora que se ejecutara a las 12:00
            //0120 -> Tiempo de riego
            //0000 -> Tiempo de abono
            Random rnd = new Random();
            int numrandom=rnd.Next(1,15);


            string programas = string.Empty;

            for (int i=0;i<numrandom;i++)
            {
                Dia dias = Dia.None;
                int veces = rnd.Next(0, 7);
                for (int j=0;j< veces; j++)
                {
                    System.DateTime dt = System.DateTime.Now + TimeSpan.FromHours( rnd.Next(0, 2299));
                    dias |= Programa.GetDia(dt.DayOfWeek);
                }

                Programa program = new Programa()
                {
                    Sector = i+1,
                    Dias = dias,
                    Hora = new TimeSpan(0, rnd.Next(0, 23), rnd.Next(0, 59), 0),
                    TiempoRiego = TimeSpan.FromHours(rnd.Next(0, 99)) + TimeSpan.FromMinutes( rnd.Next(0,59)),
                    TiempoAbono = TimeSpan.FromHours(rnd.Next(0, 99)) + TimeSpan.FromMinutes(rnd.Next(0, 59)),


                };


                programas += "+P:" + program.ToString() + "\r\n";
            }

            return Ok("+R:O\r\n+C:12765331679911111215\r\n"+programas);
            //return Ok(new { count = 1, submitted.Length, filePath });
        }


        // POST api/values
        [HttpPost]
        public void Post([FromBody]string value)
        {
        }

        // PUT api/values/5
        [HttpPut("{id}")]
        public void Put(int id, [FromBody]string value)
        {
        }

        // DELETE api/values/5
        [HttpDelete("{id}")]
        public void Delete(int id)
        {
        }


    }
}
