// One of the goals of mkvsynth is to have an efficient way to seek to a specific frame in a file
// There are a lot of problems doing this, but fseek (go to a specific byte in a file) and ftell (get what byte we are on) should help with this.
// One of the big problems still remaining is that you can't read any frame you want, you have to start at an 'I' frame.
// A complication is that there are different video codecs
// This is not an mvp feature, therefore it isn't on the issue tracker.
