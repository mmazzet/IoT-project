require('dotenv').config();
console.log(process.env.API_KEY);

// Firebase configuration
const firebaseConfig = {
  apiKey: process.env.API_KEY,
  authDomain: process.env.AUTH_DOMAIN,
  databaseURL: process.env.DATABASE_URL,
  projectId: process.env.PROJECT_ID,
  storageBucket: process.env.STORAGE_BUCKET,
  messagingSenderId: process.env.MESSAGING_SENDERID,
  appId: process.env.APP_ID
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
