let freeMemory = formatted(os.freemem() * (1/1024) * (1/1024) * (1/1024)) // in GiB
let totalMemory = formatted(os.totalmem() * (1/1024) * (1/1024) * (1/1024)) // in GiB

console.debug(`Free Memory: ${freeMemory} GiB`)
console.debug(`Total Memory: ${totalMemory} GiB`)
console.debug(`Ratio: ${formatted(freeMemory/totalMemory) * 100}%`)