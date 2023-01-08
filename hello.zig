const std = @import("std");

pub fn main() !void {
    const args = std.process.args();
    const stdout = std.io.getStdOut().writer();
    try stdout.print("hello {d}\n", .{args.inner.count});
}
