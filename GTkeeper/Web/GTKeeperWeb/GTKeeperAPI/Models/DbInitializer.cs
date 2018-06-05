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
             
        }
    }
}
