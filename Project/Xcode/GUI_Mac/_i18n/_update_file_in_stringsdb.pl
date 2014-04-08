#!/usr/bin/perl -w

#tested on Perl v5.12.3
#Author: Max Pozdeev

use Encode qw/encode decode/;
use File::Basename;
use Data::Dumper;

my $stringsdb = "./stringsdb.txt";

if(int @ARGV == 0) {
    die "Usage: $0 <filename>";
}
my $filename = $ARGV[0];
my %db;


if(-f $stringsdb) {
    #print "Loading db\n";
    open(my $in, '<:raw', $stringsdb) || die "Couldn't open db file: $!";
    my $text = do { local $/; <$in> };
    close $in;
    my $content = decode('UTF-16', $text);
    my @lines = split /\n/, $content;
    foreach(@lines) {
        
        if($_ =~ /^(\S+?)\s*=\s*(.+)/) {
            my $fileparam = $1;
            my $value = $2;
            my($file,$param)=split /\//, $fileparam, 2;
            $db{$file}{$param} = $value;
        }
    }
}

#open .strings file
addStringsFromFile($filename);
#print Dumper(\%db);


#save stringsdb
my $output_str = "";
foreach(sort keys %db) {
    my $fileIndex = $_;
    my $fileHash = \%{$db{$fileIndex}};
    foreach(sort keys %$fileHash) {
        my $param = $_;
        my $value = $fileHash->{$param};
        #print "$param $value\n";
        $output_str .= "$fileIndex/$param=$value\n";
    }
}

open my $out, '>:raw', $stringsdb;
print $out encode("UTF-16", $output_str);
close $out;


sub addStringsFromFile
{
    my ($filename) = @_;
    my $fileIndex = basename($filename);
    
    open(my $in, '<:raw', $filename) || die "Couldn't open $filename: $!";
    my $text = do { local $/; <$in> };
    close $in;
    my $content = decode('UTF-16', $text);
    my @lines = split /\n/, $content;

    foreach(@lines) {
        
        if($_ =~ /^"(.+?)"\s*=\s*"(.+)"/) {
            my $param = $1;
            my $value = $2;
            #print "found $param $value\n";
            
            if(!defined $db{$fileIndex}{$param}) {
                $db{$fileIndex}{$param} = "csv/$param//$value";
                print "Added $param from $fileIndex: ". encode('utf-8',$value) ."\n";
            }
            else {
                my @dbValue = split(/\//, $db{$fileIndex}{$param}, 4);
                if($dbValue[3] ne $value) {
                    $db{$fileIndex}{$param} = "$dbValue[0]/$dbValue[1]/$dbValue[2]/$value";
                    print "Replaced $param in $fileIndex: ". encode('utf-8',$value) ."\n";
                }
            }
        }
    }
    print "Parsed $filename\n";
}