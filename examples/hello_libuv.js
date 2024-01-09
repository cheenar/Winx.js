let freeMemory = formatted(os.freemem() * 0.000000001) // in GiB
let totalMemory = formatted(os.totalmem() * 0.000000001) // in GiB

console.debug(`Free Memory: ${freeMemory} GiB`)
console.debug(`Total Memory: ${totalMemory} GiB`)
console.debug(`Ratio: ${formatted(freeMemory/totalMemory) * 100}%`)