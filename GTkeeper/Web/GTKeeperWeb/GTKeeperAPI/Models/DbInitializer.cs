using GTKeeperAPI.Models.Identity;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace GTKeeperAPI.Models
{
    public static class DbInitializer
    {

        public static async void InitializeAsync(GTKeeperContext context)//SchoolContext is EF context
        {

            context.Database.EnsureCreated();//if db is not exist ,it will create database .but ,do nothing .

            // Look for any students.
            if (context.Users.Any())
            {
                return;   // DB has been seeded
            }

            var user =
            new GTKeeperUser
            {
                UserName = "Email@email.com",
                NormalizedUserName = "email@email.com",
                Email = "Email@email.com",
                NormalizedEmail = "email@email.com",
                EmailConfirmed = true,
                LockoutEnabled = false,
                SecurityStamp = Guid.NewGuid().ToString(),
                FirstName = "Gabriel",
                LastName = "Tamé"

            };
            

            var roleStore = new RoleStore<IdentityRole>(context);
            
            if (!context.Roles.Any(r => r.Name == "admin"))
            {
                await roleStore.CreateAsync(new IdentityRole { Name = "admin", NormalizedName = "admin" });
            }

            if (!context.Users.Any(u => u.UserName == user.UserName))
            {
                var password = new PasswordHasher<GTKeeperUser>();
                var hashed = password.HashPassword(user, "password");
                user.PasswordHash = hashed;
                var userStore = new UserStore<GTKeeperUser>(context);
                await userStore.CreateAsync(user);
                await userStore.AddToRoleAsync(user, "admin");
            }

            await context.SaveChangesAsync();




            //creamos un device
            var device = new Device()
            {
              Imei = "123456789012345",
              Nombre = "Test1",
              AvisosSMS = SmsAviso.SMSNone,
              MovilAviso = "653316799",
              MotorDiesel = false,
              NumAbono = 1,
              NumPuertos = 15,

            };
            context.Devices.Add(device);
            await context.SaveChangesAsync();


              //Generamos programas para cada device

              Random rnd = new Random();
      //03101010120001200000
      //03->Sector
      //127-> Dias * Dias que se ejecutara (Martes,Jueves,Sabado)
      //1200 -> Ejecucion * Hora que se ejecutara a las 12:00
      //0120 -> Tiempo de riego
      //0000 -> Tiempo de abono

              for (int i = 0;i < Device.MAX_PROGRAMAS;i++)
              {


                Dia dias = Dia.None;
                int veces = rnd.Next(0, 7);
                for (int j = 0; j < veces; j++)
                {
                  System.DateTime dt = System.DateTime.Now + TimeSpan.FromHours(rnd.Next(0, 2299));
                  dias |= Programa.GetDia(dt.DayOfWeek);
                }


                  Programa programa = new Programa();
                programa.Sector = i+1;
                programa.Dias = dias;
                programa.Hora = new TimeSpan(0, rnd.Next(0, 23), rnd.Next(0, 59), 0);
                programa.TiempoRiego = TimeSpan.FromHours(rnd.Next(0, 99)) + TimeSpan.FromMinutes(rnd.Next(0, 59));
                programa.TiempoAbono = TimeSpan.FromHours(rnd.Next(0, 99)) + TimeSpan.FromMinutes(rnd.Next(0, 59));
                programa.Device = device;
 
         

                device.Programas.Add(programa);
              }
              await context.SaveChangesAsync();

         }
    }
}
