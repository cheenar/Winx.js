const val = Winx.os.stdin("Enter a positive number: ")
const number = Number.parseInt(val);
let isPrime = true;

// check if number is equal to 1
if (number === 1) {
  console.log("1 is neither prime nor composite number.");
}

// check if number is greater than 1
else if (number > 1) {

  // looping through 2 to number-1
  for (let i = 2; i < number / 2; i++) {
    if (number % i == 0) {
      isPrime = false;
      break;
    }
  }

  if (isPrime) {
    console.log(`${number} is a prime number`);
  } else {
    console.log(`${number} is a not prime number`);
  }
}