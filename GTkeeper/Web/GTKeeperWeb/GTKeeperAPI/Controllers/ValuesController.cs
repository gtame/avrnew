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
        private readonly GTKeeperContext _dbContext;

        public ValuesController(GTKeeperContext dbContext)
        {
            _dbContext = dbContext;
        
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
                    NumAbono = 1,
                    NumPuertos = 15,

                }
            );
            
            Random rnd = new Random();
            //Generamos programas para cada device
            foreach (var device in devices)
            {

                //03101010120001200000
                //03->Sector
                //127-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
                //1200 -> Ejecucion * Hora que se ejecutara a las 12:00
                //0120 -> Tiempo de riego
                //0000 -> Tiempo de abono
                for (int i = 0; i < device.NumPuertos*2; i++)
                {
                    Dia dias = Dia.None;
                    int veces = rnd.Next(0, 7);
                    for (int j = 0; j < veces; j++)
                    {
                        System.DateTime dt = System.DateTime.Now + TimeSpan.FromHours(rnd.Next(0, 2299));
                        dias |= Programa.GetDia(dt.DayOfWeek);
                    }

                    Programa program = new Programa()
                    {
                        Sector = i + 1,
                        Dias = dias,
                        Hora = new TimeSpan(0, rnd.Next(0, 23), rnd.Next(0, 59), 0),
                        TiempoRiego = TimeSpan.FromHours(rnd.Next(0, 99)) + TimeSpan.FromMinutes(rnd.Next(0, 59)),
                        TiempoAbono = TimeSpan.FromHours(rnd.Next(0, 99)) + TimeSpan.FromMinutes(rnd.Next(0, 59)),


                    };


                }

            }
           
        }
         

        //Obtiene 
        // GET api/values/5
        [HttpGet("{imei}")]
        public Device GetInfo(string imei)
        {
            return  devices[imei];
        }


        [HttpGet("commit")]
        public IActionResult Commit(string imei)
        {
            //Obtenemos el device
            Device device = GetInfo(imei);
            if (device == null)
                return NotFound($"Dispositivo no encontrado {imei}");

            device.CommitChanges();

            return Ok();
        }

        [HttpGet("programacion")]
        public IActionResult Programacion2(string imei, string lup, string luc)
        {
            string result = string.Empty;
            //Obtenemos el device
            Device device = GetInfo(imei);
            if (device == null)
                return NotFound($"Dispositivo no encontrado {imei}");

            
            //Si tiene alguna salida activa la desactivamos - Forzandolas
            device.ClearSalidas();

            //Fijamos la hora de la ultima sincronizacion
            device.LastSync = System.DateTime.Now;

            //Chequeamos si esta pdte de sincronizar la config
            if (device.IsPendingConfig(luc))
                result += "\r\n" + device.ToString();

            //Chequeamos si esta pdte de sincronizar la programación
            if (device.IsPendingProgram(lup))
                result += "\r\n" + device.Programas.ToString();


            return Ok(result);
        }


        [HttpPost("programacion")]
        public  IActionResult Programacion(string imei,string lup,string luc,IFormFile submitted)
        {
            string result = string.Empty;
             
            Device device = GetInfo(imei);
            if (device == null)
                return NotFound($"Dispositivo no encontrado {imei}");


            //Obtiene el archivo en el metodo post
            var filePath = Path.GetTempFileName();
            if (submitted.Length > 0)
            {
                using (var stream = new FileStream(filePath, FileMode.Create))
                {
                    submitted.CopyTo(stream);
                }
            }
            else
                return NotFound($"No hay disponible información de archivo");
            


            //Parseamos el archivo entrante
            string[] lines = System.IO.File.ReadAllLines(filePath);
            if (!device.ParseFile(lines,lup,luc))
                result += "+R:E";
            else
                result += "+R:O";

 
            //Chequeamos si esta pdte de sincronizar la config
            if (device.IsPendingConfig(luc))
                result += "\r\n" + device.ToString();

            //Chequeamos si esta pdte de sincronizar la programación
            if (device.IsPendingProgram(lup))
                result += "\r\n" + device.Programas.ToString();


            //Todo recibido OK
            //return Ok("+R:O\r\n");
            //REcibido Error! :(
            //return Ok("+R:E\r\n");

            //Recibido y pasamos configuracion
            //return Ok("+R:O\r\n+C:12765331679911111215\r\n");



            //Recibido y pasamos programacion
            //return Ok("+R:O\r\n+C:12765331679911111215\r\n+D:O\r\n");


            //string programas = "+P:" + program.ToString() + "\r\n";
            return Ok(result + "\r\n");

            
            //return Ok("+R:O\r\n+C:12765331679911111215\r\n"+"");
            //return Ok(new { count = 1, submitted.Length, filePath });
        }

        /*
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

    */
    }
}


