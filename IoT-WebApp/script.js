// Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyBb-ncbFYFbOVZgShrcFD95S5foYF100MU",
  authDomain: "iot-roomtempconsys.firebaseapp.com",
  databaseURL:
    "https://iot-roomtempconsys-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "iot-roomtempconsys",
  storageBucket: "iot-roomtempconsys.appspot.com",
  messagingSenderId: "877752628868",
  appId: "1:877752628868:web:46cb4add72c645b53f34d1",
};

firebase.initializeApp(firebaseConfig);

// Get a reference to the database service
const database = firebase.database();

// Create database reference
const tempRef = database.ref("events/temperature");

// Sync on any updates to the DB. THIS CODE RUNS EVERY TIME AN UPDATE OCCURS ON THE DB.
tempRef.limitToLast(1).on("value", function (reading) {
  reading.forEach(function (readingTemp) {
    const event = readingTemp.val()["event"];
    const temp = readingTemp.val()["temperature"];

    document.getElementById(
      "events"
    ).innerText += `Event: ${event}, Temp: ${temp}  \n`;
  });
});
