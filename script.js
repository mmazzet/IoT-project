// Firebase configuration
const firebaseConfig = {
  apiKey: "xx",
  authDomain: "xx",
  databaseURL: "xx",
  projectId: "xx",
  storageBucket: "xx",
  messagingSenderId: "xx",
  appId: "xx"
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
    ).innerText += `Event: ${event}, Temp: ${temp}\n`;
  });
});