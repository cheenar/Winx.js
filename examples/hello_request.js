for (const [key, value] of Object.entries(request)) {
  console.log("\033[22;0;0t" + `K(${key}): V(${value})`); 
}

