// btn get started DOM
const btn = document.querySelector("#str");
const btn2 = document.querySelector("#btn-str");
const front = document.querySelector(".front");

btn.addEventListener("click", function () {
  front.classList.toggle("slide");
});
btn2.addEventListener("click", function () {
  front.classList.toggle("slide");
});

// Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyBPuPVeW39jFjpKpgJhfBC8kF8iW10-XWo",
  authDomain: "takhir-f0aab.firebaseapp.com",
  databaseURL:
    "https://takhir-f0aab-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "takhir-f0aab",
  storageBucket: "takhir-f0aab.appspot.com",
  messagingSenderId: "1015958444295",
  appId: "1:1015958444295:web:1d5f3302bee951ef380da2",
};

// Initialize firebase
firebase.initializeApp(firebaseConfig);
const database = firebase.database();

// DOM
const Suhu = document.querySelector(".tsuhu");
const Kelembaban = document.querySelector(".tkelembaban");
const Cahaya = document.querySelector(".tcahaya");
var table = document.getElementById("myTable");

// date
function waktu() {
  var year = new Date().getFullYear();
  var mount = new Date().getMonth() + 1;
  var day = new Date().getDate();
  var hour = new Date().getHours();
  var minute = new Date().getMinutes();
  dateTime = year + "-" + mount + "-" + day + "-" + hour + ":" + minute;
  return dateTime;
}

// load suhu
const dbRumahKaca = database.ref("rumah-kaca").on("value", add);
function add(snapshot) {
  var db = snapshot.val();
  var dbSuhu = db.Suhu;
  var dbKelembaban = db.Kelembaban;
  var dbCahaya = db.AnalogLDR;
  var ketCahaya = "";
  
  if (dbCahaya < 3000) {
    ketCahaya = "Dark";
  } else {
    ketCahaya = "Light";
  }
  
  Suhu.innerHTML = dbSuhu;
  Kelembaban.innerHTML = dbKelembaban;
  Cahaya.innerHTML = ketCahaya+" "+dbCahaya;
  
  console.log(dbCahaya);
  const ctx = document.getElementById("myChart");
  const myChart = new Chart(ctx, {
    type: "line",
    data: {
      labels: [0],
      datasets: [
        {
          label: "Temperature",
          data: [0],
          backgroundColor: ["rgba(255, 99, 132, 0.2)"],
          borderColor: ["rgba(255, 99, 132, 1)"],
          borderWidth: 1,
        },
        {
          label: "Humidity",
          data: [0],
          backgroundColor: ["rgba(54, 162, 235, 0.2)"],
          borderColor: ["rgba(54, 162, 235, 1)"],
          borderWidth: 1,
        },
      ],
    },
    options: {
      scales: {
        y: {
          beginAtZero: true,
        },
      },
    },
  });


  setInterval(function myFunction() {
    const dbRumahKaca = database.ref("rumah-kaca").on("value", function (snapshot) {
      var db = snapshot.val();
      var dbSuhu = db.Suhu;
      var dbKelembaban = db.Kelembaban;
      var dbCahaya = db.AnalogLDR;
  
      database.ref("simpan").push().set({
        Suhu: dbSuhu,
        Kelembaban: dbKelembaban,
        AnalogLDR: dbCahaya,
        Waktu: waktu(),
      });

    });


  },60000);

  database.ref("simpan").on("child_added", function (snapshot) {
    var db = snapshot.val();
    var dbSuhu = db.Suhu;
    var dbKelembaban = db.Kelembaban;
    var dbCahaya = db.AnalogLDR;

    var row = table.insertRow(table.rows.length);
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    var cell3 = row.insertCell(2);
    var cell4 = row.insertCell(3);
    var newsuhu = document.createTextNode(dbSuhu);
    var newcahaya = document.createTextNode(dbCahaya);
    var newkelembaban = document.createTextNode(dbKelembaban);
    var newwaktu = document.createTextNode(waktu());
    cell1.appendChild(newsuhu);
    cell2.appendChild(newcahaya);
    cell3.appendChild(newkelembaban);
    cell4.appendChild(newwaktu);

    myChart.data.datasets[0].data.push(dbSuhu);
    myChart.data.datasets[1].data.push(dbKelembaban);

    myChart.data.labels.push(waktu());

    myChart.update();
  });
}
