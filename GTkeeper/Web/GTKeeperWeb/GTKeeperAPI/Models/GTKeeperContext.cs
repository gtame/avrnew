using Microsoft.AspNetCore.Identity.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using GTKeeperAPI.Models.Identity;

namespace GTKeeperAPI.Models
{
    public class GTKeeperContext : IdentityDbContext<GTKeeperUser>
    {
        public GTKeeperContext()
        {

        }

        public DbSet<Device> Devices { get; set; }
        public DbSet<Log> Logs { get; set; }
        public DbSet<Programa> Programas { get; set; }
        public DbSet<Salida> Salidas { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);
        
        }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            //   optionsBuilder.UseSqlServer(@"Server=.;Database=SchoolDB;Trusted_Connection=True;");

            optionsBuilder.UseSqlite("Data Source=blogging.db");
        }

    }
}
