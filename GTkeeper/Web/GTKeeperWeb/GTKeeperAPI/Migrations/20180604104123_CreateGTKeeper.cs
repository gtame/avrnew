using System;
using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Migrations;

namespace GTKeeperAPI.Migrations
{
    public partial class CreateGTKeeper : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Devices",
                columns: table => new
                {
                    ID = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn),
                    Nombre = table.Column<string>(nullable: true),
                    Imei = table.Column<string>(nullable: true),
                    MovilAviso = table.Column<string>(nullable: true),
                    PasswordSMS = table.Column<string>(nullable: true),
                    AvisosSMS = table.Column<int>(nullable: false),
                    NumPuertos = table.Column<int>(nullable: false),
                    NumAbono = table.Column<int>(nullable: false),
                    MotorDiesel = table.Column<bool>(nullable: false),
                    IsConfigPendingSync = table.Column<bool>(nullable: false),
                    LastUpdateConfig = table.Column<DateTime>(nullable: false),
                    IsProgramPendingSync = table.Column<bool>(nullable: false),
                    LastUpdateProgram = table.Column<DateTime>(nullable: false),
                    LastCommitDate = table.Column<DateTime>(nullable: true),
                    LastSync = table.Column<DateTime>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Devices", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Logs",
                columns: table => new
                {
                    LogID = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn),
                    Type = table.Column<int>(nullable: false),
                    Mensaje = table.Column<string>(nullable: true),
                    Fecha = table.Column<DateTime>(nullable: false),
                    Registrado = table.Column<DateTime>(nullable: false),
                    Source = table.Column<int>(nullable: false),
                    DeviceID = table.Column<int>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Logs", x => x.LogID);
                    table.ForeignKey(
                        name: "FK_Logs_Devices_DeviceID",
                        column: x => x.DeviceID,
                        principalTable: "Devices",
                        principalColumn: "ID",
                        onDelete: ReferentialAction.Restrict);
                });

            migrationBuilder.CreateTable(
                name: "Programas",
                columns: table => new
                {
                    ProgramaID = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn),
                    Sector = table.Column<int>(nullable: false),
                    Dias = table.Column<int>(nullable: false),
                    Hora = table.Column<TimeSpan>(nullable: false),
                    TiempoRiego = table.Column<TimeSpan>(nullable: false),
                    TiempoAbono = table.Column<TimeSpan>(nullable: false),
                    DeviceID = table.Column<int>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Programas", x => x.ProgramaID);
                    table.ForeignKey(
                        name: "FK_Programas_Devices_DeviceID",
                        column: x => x.DeviceID,
                        principalTable: "Devices",
                        principalColumn: "ID",
                        onDelete: ReferentialAction.Restrict);
                });

            migrationBuilder.CreateTable(
                name: "Salidas",
                columns: table => new
                {
                    SalidaID = table.Column<int>(nullable: false)
                        .Annotation("SqlServer:ValueGenerationStrategy", SqlServerValueGenerationStrategy.IdentityColumn),
                    Programa = table.Column<int>(nullable: true),
                    Sector = table.Column<int>(nullable: true),
                    Tipo = table.Column<int>(nullable: false),
                    Desde = table.Column<DateTime>(nullable: false),
                    Hasta = table.Column<DateTime>(nullable: true),
                    DeviceID = table.Column<int>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Salidas", x => x.SalidaID);
                    table.ForeignKey(
                        name: "FK_Salidas_Devices_DeviceID",
                        column: x => x.DeviceID,
                        principalTable: "Devices",
                        principalColumn: "ID",
                        onDelete: ReferentialAction.Restrict);
                });

            migrationBuilder.CreateIndex(
                name: "IX_Logs_DeviceID",
                table: "Logs",
                column: "DeviceID");

            migrationBuilder.CreateIndex(
                name: "IX_Programas_DeviceID",
                table: "Programas",
                column: "DeviceID");

            migrationBuilder.CreateIndex(
                name: "IX_Salidas_DeviceID",
                table: "Salidas",
                column: "DeviceID");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Logs");

            migrationBuilder.DropTable(
                name: "Programas");

            migrationBuilder.DropTable(
                name: "Salidas");

            migrationBuilder.DropTable(
                name: "Devices");
        }
    }
}
